import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import path from 'node:path';
import {spawn} from 'node:child_process';
import {once} from 'node:events';
import {chromium} from 'playwright';

const root = path.resolve(import.meta.dirname, '..');
process.chdir(root);
const port = process.env.DP_AIDS_QA_PORT || '5198';
const origin = `http://127.0.0.1:${port}`;
const dir = 'build/design-patterns-discussion-aids-qa';
await fs.mkdir(dir, {recursive:true});
const server = spawn(process.execPath, ['scripts/serve.mjs'], {env:{...process.env, PORT:port, BASE_PATH:'/'}, stdio:['ignore','pipe','inherit']});
let browser;
try {
  await once(server.stdout, 'data');
  browser = await chromium.launch({headless:true,
    ...(process.env.CHROMIUM_PATH ? {executablePath:process.env.CHROMIUM_PATH} : {}),
    args:['--no-sandbox','--disable-dev-shm-usage']});
  const page = await browser.newPage({viewport:{width:1440,height:1000}});
  const errors = [];
  page.on('pageerror', error => errors.push(error.message));
  const report = {chapters:0, excerpts:0, mobileChecks:0, errors};
  const go = async (chapter,index) => {
    await page.goto(`${origin}/?course=design-patterns-cpp&chapter=${chapter}&view=slides&slide=${index+1}`);
    await page.locator('.dp-discussion-aid').waitFor({timeout:60000});
    await page.waitForFunction(() => document.querySelector('.dp-aid-figure img')?.naturalWidth > 0);
  };
  const fit = async selector => {
    const size = await page.locator(selector).evaluate(img => {
      const r = img.getBoundingClientRect(), p = img.parentElement.getBoundingClientRect();
      return {width:r.width,height:r.height,parentWidth:p.width,parentHeight:p.height,
        viewportWidth:innerWidth,viewportHeight:innerHeight,objectFit:getComputedStyle(img).objectFit,naturalWidth:img.naturalWidth};
    });
    assert.equal(size.objectFit, 'contain');
    assert.ok(size.naturalWidth > 0);
    assert.ok(size.width <= size.parentWidth+1 && size.height <= size.parentHeight+1);
    assert.ok(size.width <= size.viewportWidth+1 && size.height <= size.viewportHeight+1);
    assert.ok(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth+1));
  };
  for (let chapter=0; chapter<24; chapter++) {
    const deck = JSON.parse(await fs.readFile(`lectures/design-patterns-cpp/ch${chapter}.json`,'utf8'));
    const samples = new Map();
    deck.slides.forEach((s,i) => {if(s.designPatternsDiscussionAid && s.reading?.length) samples.set(s.designPatternsDiscussionAid.example,i);});
    for (const [example,index] of samples) {
      await go(chapter,index);
      await fit('.dp-aid-figure img');
      assert.equal(await page.locator('.dp-aid-example .slides-excerpt pre code').innerText(), deck.designPatternsDiscussionAids.examples[example].text);
      assert.ok(await page.locator('.dp-aid-reading .slides-reading').isVisible());
      report.excerpts++;
    }
    const index = [...samples.values()].at(-1);
    await page.locator('[data-action="dp-aid-zoom"]').click();
    await page.locator('.dp-aid-viewer').waitFor({state:'visible'});
    await fit('.dp-aid-viewer-image');
    await page.keyboard.press('Home');
    assert.equal(await page.locator('#slides-jump').inputValue(), String(index));
    await page.keyboard.press('Escape');
    await page.locator('.dp-aid-viewer').waitFor({state:'hidden'});
    await page.locator('[data-action="dp-aid-example"]').click();
    await page.locator('#slides-editor').waitFor();
    const id = deck.designPatternsDiscussionAids.examples[[...samples.keys()][0]].fullSlideId;
    const program = deck.slides.find(s=>s.id===id);
    assert.equal(await page.locator('#slides-editor').inputValue(), program.code.text);
    await page.locator('#slides-editor').fill(program.code.text + '\n// discussion draft check');
    await page.locator('#slides-jump').selectOption(String(index));
    await page.locator('[data-action="dp-aid-example"]').click();
    assert.match(await page.locator('#slides-editor').inputValue(), /discussion draft check/);
    report.chapters++;
    console.log(`Chapter ${chapter}: fitted diagram, excerpt, modal, and retained draft checked.`);
  }
  for (const width of [390,768]) {
    await page.setViewportSize({width,height:844});
    for (const chapter of [0,1,8,14,18,23]) {
      const deck = JSON.parse(await fs.readFile(`lectures/design-patterns-cpp/ch${chapter}.json`,'utf8'));
      const index = deck.slides.findIndex(s=>s.designPatternsDiscussionAid && s.reading?.length);
      await go(chapter,index); await fit('.dp-aid-figure img');
      await page.locator('[data-action="dp-aid-zoom"]').click();
      await fit('.dp-aid-viewer-image');
      await page.locator('[data-action="dp-aid-close"]').click();
      report.mobileChecks++;
      if (width === 390 && chapter === 23) await page.screenshot({path:dir+'/discussion-mobile.png',fullPage:true});
    }
  }
  await page.setViewportSize({width:1440,height:1000});
  const deck = JSON.parse(await fs.readFile('lectures/design-patterns-cpp/ch23.json','utf8'));
  await go(23,deck.slides.findIndex(s=>s.title==='Stage five: validation, a proposed history, and a commit point'));
  await page.screenshot({path:dir+'/discussion-desktop.png',fullPage:true});
  await page.locator('[data-action="dp-aid-zoom"]').click();
  await page.screenshot({path:dir+'/discussion-fullscreen.png'});
  assert.deepEqual(errors,[]);
  await fs.writeFile(dir+'/browser-results.json',JSON.stringify(report,null,2)+'\n');
  console.log(JSON.stringify(report));
} finally {
  if (browser) await browser.close();
  const exited = once(server,'exit'); server.kill(); await exited;
}

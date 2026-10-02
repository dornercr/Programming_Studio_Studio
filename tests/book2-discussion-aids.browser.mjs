import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import path from 'node:path';
import {spawn} from 'node:child_process';
import {once} from 'node:events';
import {chromium} from 'playwright';

const root = path.resolve(import.meta.dirname, '..');
process.chdir(root);
const port = process.env.BOOK2_AIDS_QA_PORT || '5200';
const origin = `http://127.0.0.1:${port}`;
const dir = 'build/book2-discussion-aids-qa';
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
    await page.goto(`${origin}/?course=cpp-book-02&chapter=${chapter}&view=slides&slide=${index+1}`);
    await page.locator('.b2-discussion-aid').waitFor({timeout:60000});
    await page.waitForFunction(() => document.querySelector('.b2-aid-figure img')?.naturalWidth > 0);
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
  for (let chapter=0; chapter<29; chapter++) {
    const deck = JSON.parse(await fs.readFile(`lectures/cpp-book-02/ch${chapter}.json`,'utf8'));
    const samples = new Map();
    deck.slides.forEach((s,i) => {if(s.bookTwoDiscussionAid) samples.set(s.bookTwoDiscussionAid.example,i);});
    for (const [example,index] of process.argv.includes('--layout-only')?[samples.entries().next().value]:samples) {
      await go(chapter,index);
      await fit('.b2-aid-figure img');
      assert.equal(await page.locator('.b2-aid-example .slides-excerpt pre code').innerText(), deck.bookTwoDiscussionAids.examples[example].text);
      report.excerpts++;
    }
    const runnable = [...samples].find(([key])=>deck.bookTwoDiscussionAids.examples[key]?.fullSlideId);
    const [exampleKey,index] = runnable || [...samples][0];
    await go(chapter,index);
    assert.match(await page.locator('.slides-meta .slides-eyebrow').innerText(),/BOOK II/i);
    await page.locator('[data-action="b2-aid-zoom"]').click();
    await page.locator('.b2-aid-viewer').waitFor({state:'visible'});
    await fit('.b2-aid-viewer-image');
    await page.keyboard.press('Home');
    assert.equal(await page.locator('#slides-jump').inputValue(), String(index));
    await page.keyboard.press('Escape');
    await page.locator('.b2-aid-viewer').waitFor({state:'hidden'});
    if(runnable){
    await page.locator('[data-action="b2-aid-example"]').click();
    await page.locator('#slides-editor').waitFor();
    const id = deck.bookTwoDiscussionAids.examples[exampleKey].fullSlideId;
    const program = deck.slides.find(s=>s.id===id);
    assert.equal(await page.locator('#slides-editor').inputValue(), program.code.text);
    if(program.id.startsWith('b02-discussion-program-'))
      assert.equal(await page.locator('#slides-input').inputValue(),program.code.input||'');
    await page.locator('#slides-editor').fill(program.code.text + '\n// discussion draft check');
    await page.locator('#slides-jump').selectOption(String(index));
    await page.locator('[data-action="b2-aid-example"]').click();
    assert.match(await page.locator('#slides-editor').inputValue(), /discussion draft check/);
    await page.locator('#slides-editor').fill(program.code.text);
    }else{
      assert.equal(await page.locator('[data-action="b2-aid-example"]').count(),0);
      const download=page.waitForEvent('download');
      await page.locator('[data-action="b2-aid-download"]').click();
      const downloaded=await download;
      assert.equal(await fs.readFile(await downloaded.path(),'utf8'),deck.bookTwoDiscussionAids.examples[exampleKey].sourceText);
    }
    const diagramIndex=deck.slides.findIndex(s=>s.bookTwoDiagram);
    if(diagramIndex>=0){
      await page.locator('#slides-jump').selectOption(String(diagramIndex));
      await page.waitForFunction(()=>document.querySelector('.b2-aid-figure img')?.naturalWidth>0);
      await fit('.b2-aid-figure img');
      await page.locator('[data-action="b2-aid-zoom"]').click();
      await fit('.b2-aid-viewer-image');
      await page.keyboard.press('Escape');
    }
    for(const s of deck.slides.filter(s=>s.bookTwoProjectExample)){
      await page.locator('#slides-jump').selectOption(String(deck.slides.indexOf(s)));
      assert.equal(await page.locator('.b2-discussion-aid .slides-excerpt pre code').innerText(),s.code.text);
      await page.locator('[data-action="b2-aid-example"]').click();
      const e=deck.bookTwoDiscussionAids.examples[s.bookTwoProjectExample];
      assert.equal(await page.locator('#slides-editor').inputValue(),deck.slides.find(s=>s.id===e.fullSlideId).code.text);
    }
    report.chapters++;
    console.log(`Chapter ${chapter}: fitted diagram, excerpt, modal, and retained draft checked.`);
  }
  for (const width of [390,768]) {
    await page.setViewportSize({width,height:844});
    for (const chapter of [0,1,6,13,18,21,23,24,25,26,27,28]) {
      const deck = JSON.parse(await fs.readFile(`lectures/cpp-book-02/ch${chapter}.json`,'utf8'));
      const index = deck.slides.findIndex(s=>s.bookTwoDiscussionAid);
      await go(chapter,index); await fit('.b2-aid-figure img');
      await page.locator('[data-action="b2-aid-zoom"]').click();
      await fit('.b2-aid-viewer-image');
      await page.locator('[data-action="b2-aid-close"]').click();
      report.mobileChecks++;
      if (width === 390 && chapter === 23) await page.screenshot({path:dir+'/discussion-mobile.png',fullPage:true});
    }
  }
  await page.setViewportSize({width:1440,height:1000});
  const deck = JSON.parse(await fs.readFile('lectures/cpp-book-02/ch6.json','utf8'));
  await go(6,deck.slides.findIndex(s=>s.bookTwoDiscussionAid));
  await page.screenshot({path:dir+'/discussion-desktop.png',fullPage:true});
  await page.locator('[data-action="b2-aid-zoom"]').click();
  await page.screenshot({path:dir+'/discussion-fullscreen.png'});
  assert.deepEqual(errors,[]);
  await fs.writeFile(dir+'/browser-results.json',JSON.stringify(report,null,2)+'\n');
  console.log(JSON.stringify(report));
} finally {
  if (browser) await browser.close();
  const exited = once(server,'exit'); server.kill(); await exited;
}

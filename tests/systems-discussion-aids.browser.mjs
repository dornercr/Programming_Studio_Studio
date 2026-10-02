import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import path from 'node:path';
import {spawn} from 'node:child_process';
import {once} from 'node:events';
import {chromium} from 'playwright';

const root = path.resolve(import.meta.dirname, '..');
process.chdir(root);
const port = String(process.env.SYSTEMS_AIDS_QA_PORT || 5197);
const base = '/Programming_Studio_Studio/';
const origin = `http://127.0.0.1:${port}`;
const dir = 'build/systems-discussion-aids-qa';
await fs.mkdir(dir, {recursive:true});
const server = spawn(process.execPath, ['scripts/serve.mjs'], {
  env:{...process.env, PORT:port, BASE_PATH:base}, stdio:['ignore','pipe','inherit']
});
let browser;
try {
  await once(server.stdout, 'data');
  browser = await chromium.launch({headless:true,
    ...(process.env.CHROMIUM_PATH ? {executablePath:process.env.CHROMIUM_PATH} : {}),
    args:['--no-sandbox','--disable-dev-shm-usage']});
  const page = await browser.newPage({viewport:{width:1440,height:1000}});
  const errors = [], external = [];
  page.on('pageerror', error => errors.push(error.message));
  await page.route('**/*', route => {
    if (new URL(route.request().url()).origin === origin) return route.continue();
    external.push(route.request().url()); return route.abort();
  });
  const report = {chapters:61, diagramsRendered:0, exampleLinksChecked:0, mobileChecks:0, errors};
  const go = async (chapter, index) => {
    await page.goto(`${origin}${base}?course=systems-programming&chapter=${chapter}&view=slides&slide=${index+1}`);
    await page.locator('#app[aria-busy="false"]').waitFor({timeout:60000});
    await page.locator('.systems-discussion-aid').waitFor();
  };
  const imageReady = () => page.waitForFunction(() => {
    const img = document.querySelector('.systems-aid-figure img');
    return img?.complete && img.naturalWidth > 0;
  });
  const bounded = async label => assert.ok(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth+1), label);
  for (let chapter=1; chapter<=61; chapter++) {
    const deck = JSON.parse(await fs.readFile(`lectures/systems-programming/ch${chapter}.json`, 'utf8'));
    const samples = new Map();
    deck.slides.forEach((s,i) => { if(s.discussionAid && !samples.has(s.discussionAid.graph)) samples.set(s.discussionAid.graph,i); });
    const first = [...samples.values()][0];
    await go(chapter, first);
    await page.locator('[data-action="systems-aid-zoom"]').click();
    const viewer = page.locator('.systems-aid-viewer');
    await viewer.waitFor({state:'visible'});
    const fit = await page.locator('.systems-aid-viewer-image').evaluate(img => {
      const rect = img.getBoundingClientRect();
      return {width:rect.width,height:rect.height,viewportWidth:innerWidth,viewportHeight:innerHeight,
        naturalWidth:img.naturalWidth,naturalHeight:img.naturalHeight,objectFit:getComputedStyle(img).objectFit};
    });
    assert.ok(fit.width <= fit.viewportWidth && fit.height <= fit.viewportHeight, `Diagram must fit viewport: chapter ${chapter}`);
    assert.equal(fit.objectFit,'contain');
    assert.ok(fit.naturalWidth > 0 && fit.naturalHeight > 0);
    await bounded(`Zoom overflow: chapter ${chapter}`);
    if (chapter % 2) {
      await page.keyboard.press('Escape');
    } else {
      await viewer.locator('[data-action="systems-aid-viewer-close"]').click();
    }
    await viewer.waitFor({state:'hidden'});
    for (const index of samples.values()) {
      await page.locator('#slides-jump').selectOption(String(index));
      await imageReady();
      await bounded(`Desktop overflow: chapter ${chapter}, slide ${index}`);
      assert.equal(await page.locator('.systems-aid-example pre code').first().innerText(), deck.discussionAids.example.text);
      report.diagramsRendered++;
    }
    await page.locator('[data-action="systems-aid-example"]').click();
    await page.locator('#slides-editor').waitFor();
    const example = deck.slides.find(s => s.id===deck.discussionAids.example.fullSlideId);
    assert.equal(await page.locator('#slides-editor').inputValue(), example.code.text);
    assert.equal(await page.locator('#slides-jump').inputValue(), String(deck.slides.indexOf(example)));
    report.exampleLinksChecked++;
    console.log(`Chapter ${chapter}: ${samples.size} diagram variants and example link checked.`);
  }
  for (const width of [390,768]) {
    await page.setViewportSize({width,height:844});
    for (const chapter of [1,13,23,34,45,46,50,61]) {
      const deck = JSON.parse(await fs.readFile(`lectures/systems-programming/ch${chapter}.json`, 'utf8'));
      const index = deck.slides.findIndex(s => s.discussionAid && s.reading?.length);
      await go(chapter,index); await imageReady(); await bounded(`Mobile ${width}: chapter ${chapter}`);
      const rect = await page.locator('.systems-aid-figure img').boundingBox();
      assert.ok(rect.width >= 200 && rect.height > 100);
      await page.locator('.systems-aid-reading > summary').click();
      assert.ok(await page.locator('.systems-aid-reading .slides-reading').isVisible());
      report.mobileChecks++;
      if (width===390 && chapter===45) await page.screenshot({path:dir+'/discussion-mobile.png',fullPage:true});
    }
  }
  await page.setViewportSize({width:1440,height:1000});
  const deck = JSON.parse(await fs.readFile('lectures/systems-programming/ch45.json','utf8'));
  const index = deck.slides.findIndex(s => s.title==='A data-race-free lost update');
  await go(45,index); await imageReady();
  await page.screenshot({path:dir+'/discussion-desktop.png',fullPage:true});
  await page.locator('[data-action="systems-aid-example"]').click();
  const original = await page.locator('#slides-editor').inputValue();
  await page.locator('#slides-editor').fill(original+'\n// aid-link draft retention check');
  await page.locator('#slides-jump').selectOption(String(index));
  await page.locator('[data-action="systems-aid-example"]').click();
  assert.match(await page.locator('#slides-editor').inputValue(), /aid-link draft retention check/);
  await page.reload(); await page.locator('#slides-editor').waitFor();
  assert.match(await page.locator('#slides-editor').inputValue(), /aid-link draft retention check/);
  assert.deepEqual(errors,[]); assert.deepEqual(external,[]);
  report.draftsPreserved = true; report.externalRequests = 0;
  await fs.writeFile(dir+'/browser-results.json',JSON.stringify(report,null,2)+'\n');
  console.log(JSON.stringify(report,null,2));
} finally {
  if(browser) await browser.close();
  server.kill(); await once(server,'exit');
}

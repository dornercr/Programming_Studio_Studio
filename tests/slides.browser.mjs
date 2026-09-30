// Run after npm run build. External compiler traffic is always intercepted.
// CHROMIUM_PATH is optional; otherwise Playwright uses its installed browser.
// BASE_PATH can reproduce a GitHub Pages project subdirectory.
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import http from 'node:http';
import path from 'node:path';
import {once} from 'node:events';
import {chromium} from 'playwright';

const root = path.resolve(import.meta.dirname, '..');
const output = path.resolve(process.env.SLIDES_QA_DIR || path.join(root, 'build/slides-browser'));
const dist = path.join(root, 'dist');
const baseName = (process.env.BASE_PATH || 'Programming_Studio_Studio').replace(/^\/+|\/+$/g, '');
const base = baseName ? '/' + baseName + '/' : '/';
const deck = JSON.parse(await fs.readFile(path.join(root, 'lectures/cpp-book-01/ch1.json'), 'utf8'));
assert.equal(deck.slides.length, 26, 'Chapter 1 must include all 26 slides.');
await fs.mkdir(output, {recursive:true});
const types = {'.html':'text/html', '.js':'text/javascript', '.css':'text/css', '.json':'application/json', '.svg':'image/svg+xml', '.png':'image/png'};
const server = http.createServer(async (req, res) => {
  try {
    const url = new URL(req.url, 'http://127.0.0.1');
    if (!url.pathname.startsWith(base)) {res.writeHead(404).end(); return;}
    const relative = decodeURIComponent(url.pathname.slice(base.length)) || 'index.html';
    const file = path.resolve(dist, relative);
    if (!file.startsWith(dist + path.sep)) {res.writeHead(403).end(); return;}
    const data = await fs.readFile(file);
    res.writeHead(200, {'Content-Type':types[path.extname(file)] || 'application/octet-stream', 'Cache-Control':'no-cache'}).end(data);
  } catch {res.writeHead(404).end('Not found: build the application first.');}
});
server.listen(0, '127.0.0.1');
await once(server, 'listening');
const origin = `http://127.0.0.1:${server.address().port}`;
const url = origin + base;
let browser;
const checks = [], errors = [], requests = [], compilerRequests = [], unexpectedExternal = [];
let compilerMode = 'success', releaseDelayed, delayedArrived;
try {
  browser = await chromium.launch({headless:true, ...(process.env.CHROMIUM_PATH ? {executablePath:process.env.CHROMIUM_PATH} : {}), args:['--no-sandbox', '--disable-dev-shm-usage']});
  const context = await browser.newContext({viewport:{width:1512,height:1100}, reducedMotion:'reduce'});
  await context.route('**/*', async route => {
    const request = route.request();
    const requestURL = new URL(request.url());
    if (requestURL.origin === origin) return route.continue();
    if (requestURL.origin === 'https://godbolt.org' && requestURL.pathname.includes('/api/compiler/')) {
      compilerRequests.push({method:request.method(), payload:request.postDataJSON()});
      if (compilerMode === 'delay') {
        delayedArrived?.();
        await new Promise(resolve => {releaseDelayed = resolve;});
      }
      if (compilerMode === '503') return route.fulfill({status:503, contentType:'application/json', body:'{}'});
      try {
        return await route.fulfill({status:200, contentType:'application/json', body:JSON.stringify({code:0, buildResult:{code:0, stdout:[], stderr:[]}, execResult:{code:0,didExecute:true,stdout:[{text:'MOCK LIVE RESULT'}],stderr:[]}})});
      } catch (error) {
        // Aborting a request after Stop waiting can dispose its intercepted route.
        if (!/closed|handled|abort/i.test(error.message)) throw error;
      }
    }
    unexpectedExternal.push({url:request.url(), method:request.method()});
    return route.abort();
  });
  const page = await context.newPage();
  page.on('pageerror', error => errors.push(error.message));
  page.on('request', request => requests.push(request.url()));
  const ready = () => page.locator('#app[aria-busy="false"]').waitFor({timeout:30000});
  const slideNumber = () => page.locator('#slides-jump').inputValue();
  const jump = async index => {
    await page.locator('#slides-jump').selectOption(String(index));
    await page.waitForFunction(title => document.querySelector('#slides-title')?.textContent === title, deck.slides[index].title);
  };
  const mode = async id => {
    await page.locator(`#mode-tabs [data-mode="${id}"]`).click();
    await page.waitForFunction(value => new URL(location.href).searchParams.get('view') === value && document.querySelector('#app')?.getAttribute('aria-busy') === 'false', id);
    assert.equal(await page.locator('#workspace [role="alert"]').count(), 0);
  };
  const codeIndex = deck.slides.findIndex(s => s.code?.runAllowed);
  const variantIndex = deck.slides.findIndex(s => s.code?.variants?.some(v => v.standard === 'c++17'));
  assert.ok(codeIndex >= 0 && variantIndex >= 0, 'Editable examples and language-standard variants exist.');

  await page.goto(url); await ready();
  assert.ok(!requests.some(value => value.includes('/lectures/')), 'Lecture JSON is not requested on initial Outline view.');
  assert.equal(compilerRequests.length, 0);
  checks.push('Initial Outline does not fetch lecture JSON or send compiler requests.');

  await page.goto(url + '?course=cpp-book-01&chapter=1&view=slides&slide=1'); await ready();
  await page.locator('#slides-jump').waitFor();
  assert.equal(await page.locator('#slides-jump option').count(), 26);
  assert.equal(await slideNumber(), '0');
  assert.ok(requests.some(value => value.includes('/lectures/cpp-book-01/ch1.json')));
  assert.equal(await page.locator('#slides-presenter').evaluate(element => element.open), false);
  await page.screenshot({path:path.join(output, 'chapter-1-cover.png'), fullPage:true});
  await page.locator('[data-action="slides-next"]').click();
  assert.equal(await slideNumber(), '1');
  await page.locator('[data-action="slides-prev"]').click();
  assert.equal(await slideNumber(), '0');
  await page.locator('#slides-title').focus();
  await page.keyboard.press('ArrowRight'); assert.equal(await slideNumber(), '1');
  await page.keyboard.press('ArrowLeft'); assert.equal(await slideNumber(), '0');
  await page.keyboard.press('End'); assert.equal(await slideNumber(), '25');
  await page.keyboard.press('Home'); assert.equal(await slideNumber(), '0');
  await page.locator('#slides-presenter summary').click();
  assert.equal(await page.locator('#slides-presenter').evaluate(element => element.open), true);
  assert.match(await page.locator('.slides-notes').innerText(), /SAY.*verbatim narration/is);
  assert.match(await page.locator('.slides-notes').innerText(), /DO.*presenter directions/is);
  await page.locator('#slides-presenter summary').click();
  checks.push('All 26 slides, next/previous/select, arrow/Home/End navigation, and hidden presenter narration work.');

  await jump(codeIndex);
  const original = await page.locator('#slides-editor').inputValue();
  const draft = original + '\n// Browser QA saved draft\n';
  await page.locator('#slides-editor').fill(draft);
  await page.locator('#slides-editor').press('ArrowRight');
  assert.equal(await slideNumber(), String(codeIndex), 'Typing arrow keys does not change slides.');
  await page.locator('#slides-standard').selectOption('c++17');
  await page.locator('.slides-stdin summary').click();
  await page.locator('#slides-input').fill('saved input');
  await jump(codeIndex + 1); await jump(codeIndex);
  assert.equal(await page.locator('#slides-editor').inputValue(), draft);
  await page.reload(); await ready();
  assert.equal(await slideNumber(), String(codeIndex));
  assert.equal(await page.locator('#slides-editor').inputValue(), draft);
  assert.equal(await page.locator('#slides-standard').inputValue(), 'c++17');
  assert.equal(await page.locator('#slides-input').inputValue(), 'saved input');
  checks.push('Editor drafts, stdin, standard and slide position persist across navigation/reload; editor arrows do not navigate.');

  await jump(variantIndex);
  const variant = deck.slides[variantIndex].code.variants.find(v => v.standard === 'c++17');
  await page.locator('#slides-variant').selectOption(variant.id);
  assert.equal(await page.locator('#slides-standard').inputValue(), 'c++17');
  assert.equal(await page.locator('#slides-editor').inputValue(), variant.text);
  await page.locator('[data-action="slides-undo"]').click();
  assert.equal(await page.locator('#slides-editor').inputValue(), deck.slides[variantIndex].code.text);
  await page.locator('#slides-standard').selectOption('c++17');
  await page.locator('.slides-expected summary').click();
  assert.match(await page.locator('.slides-expected').innerText(), /not a live run/);
  assert.match(await page.locator('#slides-results').innerText(), /Not run/i);
  await page.locator('[data-action="slides-run"]').click();
  await page.waitForFunction(() => document.querySelector('#slides-results')?.textContent.includes('MOCK LIVE RESULT'));
  assert.match(compilerRequests.at(-1).payload.options.userArguments, /-std=c\+\+17/);
  assert.equal(compilerRequests.at(-1).method, 'POST');
  assert.ok(!(await page.locator('.slides-expected').innerText()).includes('MOCK LIVE RESULT'));
  await page.locator('#slides-editor').fill(variant.text + '\n// changed after output');
  assert.match(await page.locator('.slides-status').innerText(), /Edited since this result/);
  checks.push('Variants, undo and language controls work; expected behavior and mocked live output remain distinct; edits mark old output stale.');

  compilerMode = '503';
  await page.locator('[data-action="slides-run"]').click();
  await page.waitForFunction(() => document.querySelector('.slides-status')?.textContent.includes('HTTP 503'));
  assert.equal(await page.locator('[data-action="slides-run"]').isEnabled(), true);
  assert.match(await page.locator('#slides-editor').inputValue(), /changed after output/);
  compilerMode = 'delay';
  let arrived = new Promise(resolve => {delayedArrived = resolve;});
  await page.locator('[data-action="slides-run"]').click(); await arrived;
  assert.equal(await page.locator('[data-action="slides-stop"]').isEnabled(), true);
  await page.locator('[data-action="slides-stop"]').click();
  assert.match(await page.locator('.slides-status').innerText(), /Stopped waiting/);
  assert.equal(await page.locator('[data-action="slides-run"]').isEnabled(), true);
  releaseDelayed();
  compilerMode = 'delay';
  arrived = new Promise(resolve => {delayedArrived = resolve;});
  await page.locator('[data-action="slides-run"]').click(); await arrived;
  await jump(codeIndex);
  releaseDelayed();
  assert.ok(!(await page.locator('#slides-results').innerText()).includes('MOCK LIVE RESULT'));
  compilerMode = 'success';
  checks.push('HTTP 503 preserves the draft; Stop waiting and leaving a running slide cancel its waiting state without leaking results.');

  const stagesIndex = deck.slides.findIndex(s => s.stages?.length);
  await jump(stagesIndex);
  await page.locator('[data-action="slides-stage"]').last().click();
  const lastStage = deck.slides[stagesIndex].stages.at(-1);
  assert.match(await page.locator('.slides-workbench').innerText(), new RegExp(lastStage.artifact.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')));
  await page.locator('#slides-splitter').focus();
  const oldRatio = Number(await page.locator('#slides-splitter').getAttribute('aria-valuenow'));
  await page.keyboard.press('ArrowRight');
  assert.equal(Number(await page.locator('#slides-splitter').getAttribute('aria-valuenow')), oldRatio + 2);
  assert.equal(await slideNumber(), String(stagesIndex));
  checks.push('Build-stage selection updates the artifact; keyboard resizing changes panes without advancing the slide.');

  await jump(codeIndex);
  await page.screenshot({path:path.join(output, 'chapter-1-code.png'), fullPage:true});
  await page.locator('[data-action="slides-present"]').click();
  await page.waitForFunction(() => document.body.classList.contains('slides-presenting'));
  await page.locator('[data-action="slides-next"]').click();
  assert.equal(await page.locator('body').evaluate(el => el.classList.contains('slides-presenting')), true);
  await page.locator('#slides-title').focus(); await page.keyboard.press('Escape');
  await page.waitForFunction(() => !document.body.classList.contains('slides-presenting'));
  // Exercise browsers which decline fullscreen as well as native fullscreen.
  await jump(codeIndex);
  await page.locator('#slides-room').evaluate(room => {room.requestFullscreen = undefined;});
  await page.locator('[data-action="slides-present"]').click();
  await page.waitForFunction(() => document.body.classList.contains('slides-presenting'));
  assert.equal(await page.evaluate(() => !!document.fullscreenElement), false);
  await page.locator('#slides-editor').focus(); await page.keyboard.press('Escape');
  await page.waitForFunction(() => !document.body.classList.contains('slides-presenting'));
  await page.locator('#slides-room').evaluate(room => {delete room.requestFullscreen;});
  checks.push('Presentation survives slide navigation and exits with Escape, including CSS fallback while the editor has focus.');

  await page.setViewportSize({width:390,height:844});
  for (const index of [0, codeIndex, stagesIndex, 24]) {
    await jump(index);
    assert.ok(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth + 1), `No horizontal page overflow on mobile slide ${index + 1}.`);
    const rect = await page.locator('#slides-room').boundingBox();
    assert.ok(rect.x >= -1 && rect.x + rect.width <= 391);
  }
  await jump(codeIndex);
  await page.screenshot({path:path.join(output, 'chapter-1-mobile.png'), fullPage:true});
  await page.setViewportSize({width:1512,height:1100});
  checks.push('Cover, code, build stages and questions fit a 390px mobile viewport without horizontal page overflow.');

  await page.goto(url + '?course=cpp-book-01&chapter=2&view=slides'); await ready();
  assert.match(await page.locator('#workspace').innerText(), /No lecture for this chapter yet/);
  await page.locator('[data-action="slides-open-first"]').click();
  await page.locator('#slides-jump').waitFor(); await ready();
  assert.equal(new URL(page.url()).searchParams.get('chapter'), '1');
  assert.equal(new URL(page.url()).searchParams.get('view'), 'slides');
  await mode('outline');
  assert.ok((await page.locator('#workspace').innerText()).length > 100);
  assert.equal(await page.locator('body').evaluate(el => el.classList.contains('slides-active') || el.classList.contains('slides-presenting')), false);
  await mode('coding');
  await page.locator('#lab-editor').waitFor();
  assert.ok((await page.locator('#lab-editor').inputValue()).length > 0);
  assert.equal(await page.locator('#slides-room').count(), 0);
  checks.push('Unwritten chapters show a clear empty state and Chapter 1 recovery; existing Outline and Coding Lab still render.');

  assert.deepEqual(errors, [], 'No browser runtime errors.');
  assert.ok(!unexpectedExternal.some(r => r.method !== 'GET'), 'No unmocked external mutation request.');
  assert.equal(compilerRequests.length, 4, 'Exactly four explicit online runs; all intercepted.');
  const result = {date:new Date().toISOString(),basePath:base,checks,errors,mockedCompilerRequests:compilerRequests.length,actualExternalCompilerRequests:0,blockedExternalRequests:unexpectedExternal};
  await fs.writeFile(path.join(output, 'results.json'), JSON.stringify(result,null,2));
  console.log(checks.join('\n'));
  console.log(`PASS: ${checks.length} groups; screenshots and results: ${output}`);
} catch (error) {
  await fs.writeFile(path.join(output, 'failure.json'), JSON.stringify({message:error.message,stack:error.stack,checks,errors,compilerRequests:compilerRequests.length},null,2));
  throw error;
} finally {
  releaseDelayed?.();
  await browser?.close();
  server.close();
}

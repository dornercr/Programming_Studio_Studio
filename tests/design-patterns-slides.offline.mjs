// Build with npm run build:offline first. This test denies every network request.
import {chromium} from 'playwright';
import fs from 'node:fs/promises';
import path from 'node:path';
import {pathToFileURL} from 'node:url';
import assert from 'node:assert/strict';
const root=path.resolve(import.meta.dirname,'..');process.chdir(root);
const browser=await chromium.launch({headless:true,...(process.env.CHROMIUM_PATH?{executablePath:process.env.CHROMIUM_PATH}:{}),args:['--no-sandbox','--disable-gpu','--disable-webgl','--disable-dev-shm-usage']});
const page=await browser.newPage({viewport:{width:1500,height:1000}}),errors=[],network=[];
page.on('pageerror',e=>errors.push(e.message));page.on('request',r=>{if(/^https?:/.test(r.url()))network.push(r.url());});
await page.route('**/*',r=>/^https?:/.test(r.request().url())?r.abort():r.continue());
const ready=()=>page.locator('#app[aria-busy="false"]').waitFor({timeout:60000});
try{
 await page.goto(pathToFileURL(path.join(root,'dist-offline/index.html')).href,{timeout:90000});await ready();
 await page.locator('#book-menu-design-patterns-cpp').selectOption('0');await ready();await page.locator('#mode-tabs [data-mode="slides"]').click();await ready();
 const d=JSON.parse(await fs.readFile('lectures/design-patterns-cpp/ch0.json','utf8'));assert.equal(await page.locator('#slides-jump option').count(),d.slides.length);
 const visual=d.slides.findIndex(s=>s.visual);await page.locator('#slides-jump').selectOption(String(visual));await page.waitForFunction(()=>document.querySelector('.slides-diagram')?.naturalWidth>0);assert.ok((await page.locator('.slides-diagram').getAttribute('src')).startsWith('data:image/svg+xml;base64,'));
 await page.locator('#slides-chapter').selectOption('23');await ready();await page.waitForFunction(()=>document.querySelector('#slides-chapter')?.value==='23');
 const cap=JSON.parse(await fs.readFile('lectures/design-patterns-cpp/ch23.json','utf8'));const code=cap.slides.findIndex(s=>s.code);await page.locator('#slides-jump').selectOption(String(code));await page.locator('#slides-editor').fill(cap.slides[code].code.text+'\n// offline edit');
 await page.reload({timeout:90000});await ready();await page.locator('#mode-tabs [data-mode="slides"]').click();await ready();assert.match(await page.locator('#slides-editor').inputValue(),/offline edit/);
 const diagram=cap.slides.findIndex(s=>s.diagram?.view==='overview');await page.locator('#slides-jump').selectOption(String(diagram));await page.waitForFunction(()=>document.querySelector('.slides-diagram')?.naturalWidth>0);
 assert.deepEqual(network,[]);assert.deepEqual(errors,[]);const report={fileURL:true,chapter0DiagramEmbedded:true,capstoneDiagramEmbedded:true,chapterNavigation:true,editorAndDraftRestore:true,networkRequests:network,errors};await fs.writeFile('tests/design-patterns-slides-offline-results.json',JSON.stringify(report,null,2)+'\n');console.log(JSON.stringify(report));
}finally{await browser.close();}

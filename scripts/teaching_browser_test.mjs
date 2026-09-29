import {chooseBook,currentBook} from './book_test_helpers.mjs';
import {chromium} from 'playwright';import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath,pathToFileURL} from 'node:url';import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');const data=JSON.parse(await fs.readFile(path.join(root,'src/content.json'),'utf8'));
let options={headless:true};if(process.env.CHROMIUM_MODULE){const{default:ch}=await import(pathToFileURL(process.env.CHROMIUM_MODULE));options={...options,args:ch.args,executablePath:process.env.CHROMIUM_PATH||await ch.executablePath()};}
const b=await chromium.launch(options),p=await b.newPage({viewport:{width:1512,height:1000},reducedMotion:'reduce',acceptDownloads:true});const errors=[],results=[];p.on('pageerror',e=>errors.push(e.message));const ok=s=>{results.push(s);console.log('PASS',s)};
await p.goto(pathToFileURL(path.join(root,'dist/index.html')).href);
assert.equal(await p.locator('.chapter-guide').count(),1);assert.equal(await p.locator('#personal-note').count(),1);await p.locator('#personal-note').fill('A function parameter may be enough.');
for(const c of data.courses.filter(c=>c.teaching)){
 await chooseBook(p,c.id);
 for(const ch of c.chapters){
  await p.selectOption('#chapter-select',String(ch.number));await p.locator('#mode-tabs [data-mode="outline"]').click();const g=c.teaching[ch.number];
  assert.equal(await p.locator('.chapter-map-list>.map-section').count(),g.sections.length,`${c.id}/${ch.number} map`);
  assert.equal(await p.locator('.workshop-panel').count(),1);assert.equal(await p.locator('.trace-table tbody tr').count(),g.workshop.steps.length);
  assert.equal(await p.locator('.discussion-answer').count(),1);assert.equal(await p.locator('.expected-output code').textContent(),g.workshop.output);
  assert.equal(await p.locator('#personal-note').count(),1);assert.ok(await p.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1));
  const ids=g.sections.flatMap(s=>s.topics);const id=ids.find(id=>id!==g.startTopicId)||ids[0];
  await p.locator(`.chapter-map-list [data-action="read-topic"][data-id="${id}"]`).click();
  assert.equal(await p.locator('.lesson-explanation').count(),1);assert.equal(await p.locator('.workshop-panel').count(),1);assert.equal(await p.locator('.source-material').count(),1);
  assert.ok(!(await p.locator('.lesson-explanation').textContent()).includes('At graduate level, do not treat this as a term to memorize'));
 }
 ok(`${c.chapters.length} ${c.id} chapter maps, examples, traces, discussions, and first lessons`);
}
await chooseBook(p,'design-patterns-cpp');await p.selectOption('#chapter-select','1');await p.screenshot({path:path.join(root,'docs/preview-outline-map.png')});
await p.locator('[data-action="workshop-jump"]').first().click();await p.screenshot({path:path.join(root,'docs/preview-outline-example.png')});
await p.locator('.discussion-panel').scrollIntoViewIfNeeded();await p.locator('.discussion-answer summary').click();await p.screenshot({path:path.join(root,'docs/preview-outline-discussion.png')});
await chooseBook(p,'systems-programming');await p.selectOption('#chapter-select','30');await p.screenshot({path:path.join(root,'docs/preview-systems-map.png')});
await p.locator('.chapter-map-list [data-id="SP30.004"]').click();assert.ok((await p.locator('.lesson-heading').textContent()).includes('Cache hit'));await p.locator('#personal-note').fill('Block 4 evicts block 0 from slot 0.');await p.locator('[data-action="reviewed"]').click();await p.reload();assert.equal(await p.locator('#personal-note').inputValue(),'Block 4 evicts block 0 from slot 0.');
await p.locator('.source-material > summary').click();assert.ok((await p.locator('.source-material').textContent()).includes('Deep Dive'));await p.screenshot({path:path.join(root,'docs/preview-systems-lesson.png')});
await p.locator('.full-program summary').click();const pending=p.waitForEvent('download');await p.locator('[data-action="download-workshop"]').click();const dl=await pending;const tmp=path.join(root,'tests/workshop-download.cpp');await dl.saveAs(tmp);assert.equal(await fs.readFile(tmp,'utf8'),data.courses[1].teaching[30].workshop.code);await fs.unlink(tmp);ok('Persistent notes, source context, paired slides, and complete C++ download');
await p.locator('[data-action="chapter-map"]').first().click();await p.locator('[data-action="coverage"]').first().click();assert.equal(await p.locator('.roadmap-chapter').count(),61);await p.locator('.roadmap-chapter').nth(44).locator('summary').click();await p.locator('.roadmap-chapter').nth(44).locator('[data-action="map-chapter"]').click();assert.ok((await p.locator('.lesson-heading').textContent()).includes('Race Conditions'));ok('Course outline opens chapters and numbered sections');
await p.locator('.chapter-map-list [data-id="SP45.018"]').click();assert.equal(await p.locator('.source-correction').count(),1);assert.ok((await p.locator('.source-correction').textContent()).includes('undefined behavior'));ok('Race-condition correction visible beside the affected source lesson');
await p.setViewportSize({width:390,height:844});await p.selectOption('#chapter-select','30');await p.evaluate(()=>document.activeElement?.blur());assert.ok(await p.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1));await p.screenshot({path:path.join(root,'docs/preview-outline-mobile.png')});await p.locator('[data-action="workshop-jump"]').first().click();assert.ok(await p.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1));await p.screenshot({path:path.join(root,'docs/preview-example-mobile.png')});ok('Mobile maps, code, and trace table stay within the viewport');
await p.setViewportSize({width:1512,height:1000});await chooseBook(p,'design-patterns-cpp');await p.selectOption('#chapter-select','0');assert.equal(await p.locator('#personal-note').inputValue(),'A function parameter may be enough.');ok('Separate course notes and original topic IDs preserved');
assert.deepEqual(errors,[]);await fs.writeFile(path.join(root,'tests/teaching-browser-results.json'),JSON.stringify({results,errors},null,2));await b.close();

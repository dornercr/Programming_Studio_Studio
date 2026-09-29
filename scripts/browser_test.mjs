import {chromium} from 'playwright';
import fs from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath,pathToFileURL} from 'node:url';
import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
let options={headless:true};
if(process.env.CHROMIUM_MODULE){const mod=await import(pathToFileURL(process.env.CHROMIUM_MODULE));const ch=mod.default;options={...options,args:ch.args,executablePath:process.env.CHROMIUM_PATH||await ch.executablePath()};}
const b=await chromium.launch(options);const ctx=await b.newContext({viewport:{width:1512,height:1000},acceptDownloads:true,reducedMotion:"reduce"});const p=await ctx.newPage();const errors=[];p.on('pageerror',e=>errors.push(e.message));
const results=[];const ok=x=>{results.push(x);console.log('PASS',x)};
await p.goto(pathToFileURL(path.join(root,'dist/index.html')).href);await p.waitForSelector('.lesson-heading');
assert.equal(await p.locator('#all-count').textContent(),'539');assert.equal(await p.locator('#mode-tabs button').count(),5);ok('Offline startup and five study modes');
await p.selectOption('#chapter-select','1');await p.locator('[data-action="reviewed"]').click();await p.locator('#personal-note').fill('My Factory Method note');await p.locator('[data-action="bookmark"]').first().click();
await p.screenshot({path:path.join(root,'docs/preview-outline.png'),fullPage:false});
for(let n=0;n<=23;n++){
 await p.selectOption('#chapter-select',String(n));
 for(const mode of ['outline','flashcards','scenarios',...(n?['uml','code']:[])]){
  await p.locator(`#mode-tabs [data-mode="${mode}"]`).click();
  if(mode==='outline')assert.ok(await p.locator('.lesson-heading').count());
  if(mode==='flashcards')assert.equal(await p.locator('.card-question').count(),1);
  if(mode==='scenarios')assert.equal(await p.locator('.choice').count(),4);
  if(mode==='uml'){assert.ok(await p.locator('.uml-image').count());assert.ok(await p.locator('.theory-panel').count());assert.ok(await p.locator('.uml-image').evaluateAll(imgs=>imgs.every(i=>i.complete&&i.naturalWidth>0)));}
  if(mode==='code')assert.ok((await p.locator('.full-code').textContent()).includes('#include'));
 }
}
ok('All 24 chapters render and all 23 example chapters show UML and C++');
await p.selectOption('#chapter-select','1');await p.locator('#mode-tabs [data-mode="outline"]').click();await p.locator('#topic-nav [data-action="read-topic"][data-id="C01.01"]').click();assert.equal(await p.locator('#personal-note').inputValue(),'My Factory Method note');
await p.reload();await p.waitForSelector('.lesson-heading');assert.equal(await p.locator('#personal-note').inputValue(),'My Factory Method note');ok('Native localStorage retains notes, bookmarks and review state after reload');
await p.locator('#mode-tabs [data-mode="flashcards"]').click();await p.locator('.deck-card').click();assert.equal(await p.locator('.card-answer').count(),1);await p.screenshot({path:path.join(root,'docs/preview-flashcards.png')});await p.locator('[data-rating="again"]').click();await p.selectOption('#scope-select','review');assert.equal(await p.locator('.deck-card').count(),1);ok('Flashcard flip, rating and review queue');
await p.selectOption('#scope-select','chapter');await p.locator('#mode-tabs [data-mode="scenarios"]').click();await p.locator('[data-action="choose-answer"]').first().click();await p.locator('[data-action="check-answer"]').click();assert.equal(await p.locator('.option-rationales p').count(),4);await p.screenshot({path:path.join(root,'docs/preview-scenarios.png')});await p.locator('[data-action="retry-scenario"]').click();assert.equal(await p.locator('.choice:not([disabled])').count(),4);ok('Scenarios show four rationales and support a new attempt');
await p.locator('#mode-tabs [data-mode="uml"]').click();await p.screenshot({path:path.join(root,'docs/preview-uml.png')});await p.locator('[data-action="expand-diagram"]').first().click();assert.ok(await p.locator('dialog img').count());await p.locator('[data-action="close-dialog"]').click();ok('UML expanded viewer and adjacent code theory');
await p.locator('#mode-tabs [data-mode="code"]').click();await p.locator('[data-kind="solutions"]').click();assert.ok((await p.locator('.full-code').textContent()).includes('LengthRecord'));assert.ok((await p.locator('#workspace details').first().textContent()).includes('ch01_solutions.cpp'));const download= p.waitForEvent('download');await p.locator('[data-action="download-code"]').click();assert.equal((await download).suggestedFilename(),'ch01_solutions.cpp');ok('C++ variants and source download');
await p.locator('#book-toolbar [data-action="book-glossary"]').click();await p.locator('#glossary-search').fill('Factory Method');assert.ok((await p.locator('#glossary-results').textContent()).includes('virtual creation'));
await p.locator('[data-action="all-topics"]').click();await p.locator('#topic-search').fill('unfindable-xyz-000');assert.ok((await p.locator('#workspace').textContent()).includes('No matching'));await p.locator('[data-action="clear-filters"]').first().click();ok('Glossary and empty-search recovery');
await p.selectOption('#chapter-select','23');await p.locator('#mode-tabs [data-mode="uml"]').click();assert.equal(await p.locator('.uml-image').count(),3);await p.screenshot({path:path.join(root,'docs/preview-capstone.png')});
await p.setViewportSize({width:390,height:844});await p.screenshot({path:path.join(root,'docs/preview-mobile.png')});assert.ok(await p.evaluate(()=>document.documentElement.scrollWidth<=innerWidth+1));await p.locator('[data-action="open-menu"]').click();assert.ok((await p.locator('#sidebar').getAttribute('class')).includes('open'));await p.locator('button[data-action="close-menu"]').click();ok('Mobile layout and navigation drawer');
await p.setViewportSize({width:1512,height:1000});
await p.locator('[data-action="settings"]').first().click();const backup=p.waitForEvent('download');await p.locator('[data-action="export-backup"]').click();const dl=await backup;await dl.saveAs(path.join(root,'tests/temporary-backup.json'));await p.locator('[data-action="close-dialog"]').click();p.on('dialog',d=>d.accept());await p.locator('#import-file').setInputFiles(path.join(root,'tests/temporary-backup.json'));await p.waitForTimeout(200);assert.ok((await p.locator('#toast').textContent()).includes('restored'));await fs.unlink(path.join(root,'tests/temporary-backup.json'));ok('Export and restore backup');
await p.locator('#import-file').setInputFiles(path.join(root,'docs/example-study-pack.json'));await p.waitForFunction(()=>document.querySelector('#book-dropdowns').dataset.currentCourse==='cpp-personal-practice');assert.equal(await p.locator('#all-count').textContent(),'1');assert.equal(await p.locator('#mode-tabs button').count(),3);ok('Custom C++ study pack imports and keeps textbook-only views separate');
assert.deepEqual(errors,[]);ok('No browser JavaScript errors');await fs.writeFile(path.join(root,'tests/browser-results.json'),JSON.stringify({results,errors},null,2));await b.close();

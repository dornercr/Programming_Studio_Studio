import {sourceData,sourceCoding} from './read-source.mjs';
import {chooseBook,currentBook} from './book_test_helpers.mjs';
import {chromium} from 'playwright';import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath,pathToFileURL} from 'node:url';import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..'),data=sourceData();
let options={headless:true};if(process.env.CHROMIUM_MODULE){const{default:ch}=await import(pathToFileURL(process.env.CHROMIUM_MODULE));options={...options,args:ch.args,executablePath:process.env.CHROMIUM_PATH||await ch.executablePath()};}
const b=await chromium.launch(options),p=await b.newPage({viewport:{width:1512,height:1000},reducedMotion:'reduce'});await p.goto(process.env.STUDIO_URL||pathToFileURL(path.join(root,'dist-offline/index.html')).href);let checked=0;
for(const c of data.courses.filter(c=>c.teaching)){
 await chooseBook(p,c.id);
 for(const ch of c.chapters){
  await p.selectOption('#chapter-select',String(ch.number));
  const ts=c.topics.filter(t=>t.chapter===ch.number&&!t.outlineParent).map(t=>({id:t.id,paragraphs:t.reading.paragraphs}));
  const failures=await p.evaluate(ts=>{
   const fail=[];const normalize=s=>s.replace(/\s+/g,' ').trim();
   for(const t of ts){
    const button=document.querySelector('#topic-nav [data-action="read-topic"][data-id="'+t.id+'"]');if(!button){fail.push({id:t.id,error:'missing navigation'});continue;}button.click();
    const el=document.querySelector('.lesson-explanation > .teaching-prose');if(!el){fail.push({id:t.id,error:'missing theory'});continue;}
    const displayed=normalize(el.innerText);
    for(const paragraph of t.paragraphs)if(!displayed.includes(normalize(paragraph)))fail.push({id:t.id,error:'paragraph changed',text:paragraph.slice(0,100)});
   }
   return fail;
  },ts);
  assert.deepEqual(failures,[],`${c.id}/${ch.number}`);checked+=ts.length;
 }
 console.log('PASS',c.id,'all primary lesson explanations preserve their full text');
}
await chooseBook(p,'systems-programming');await p.selectOption('#chapter-select','30');await p.locator('.chapter-map-list').screenshot({path:path.join(root,'docs/preview-numbered-outline.png')});await p.setViewportSize({width:390,height:844});await p.locator('.chapter-map-list').scrollIntoViewIfNeeded();await p.screenshot({path:path.join(root,'docs/preview-numbered-outline-mobile.png')});
await fs.writeFile(path.join(root,'tests/reading-text-results.json'),JSON.stringify({primaryLessonsChecked:checked,paragraphsPreserved:true},null,2));console.log('Verified',checked,'rendered lessons.');await b.close();

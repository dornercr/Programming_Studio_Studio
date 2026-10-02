import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import crypto from 'node:crypto';
import vm from 'node:vm';
import {spawnSync} from 'node:child_process';
import os from 'node:os';
import path from 'node:path';
import {sourceData} from '../scripts/read-source.mjs';
const c=sourceData().courses.find(c=>c.id==='linux-system-administration');
const hash=b=>crypto.createHash('sha256').update(b).digest('hex');
const ledger=JSON.parse(fs.readFileSync('docs/linux-admin-source.json','utf8'));
const entries=JSON.parse(fs.readFileSync('lectures/manifest.json','utf8')).filter(e=>e.courseId===c.id);
test('Linux book retains all 33 chapters, 490 numbered sections, 735 exact example blocks and the original EPUB',()=>{
 assert.equal(c.chapters.filter(ch=>ch.role==='chapter').length,33);assert.equal(c.chapters.length,39);
 assert.equal(c.topics.length,761);assert.equal(c.topics.filter(t=>/^\d+\.\d+\s/.test(t.title)).length,490);
 const blocks=c.topics.flatMap(t=>t.blocks);assert.equal(blocks.length,ledger.counts.blocks);assert.equal(blocks.filter(b=>b.type==='code').length,735);
 assert.equal(hash(fs.readFileSync('content/assets/linux-admin/Linux_System_Administration.epub')),ledger.epubSHA256);
 assert.equal(hash(Buffer.from(c.linuxBook.epub.split(',')[1],'base64')),ledger.epubSHA256);
 assert.equal(new Set(c.topics.map(t=>t.id)).size,c.topics.length);assert.equal(new Set(blocks.map(b=>b.id)).size,blocks.length);
 for(const t of c.topics){assert.ok(t.blocks.length);assert.ok(t.reading.paragraphs.length);assert.ok(t.sourceRef.startsWith('EPUB/text/'));}
});
test('Every Linux chapter has a connected discussion, bounded diagram, source map and original worked example',()=>{
 const ids=new Set(c.topics.map(t=>t.id)),mapped=[];
 for(const ch of c.chapters){const g=c.teaching[ch.number];assert.ok(g.model.title&&g.model.contract&&g.model.failure);assert.equal(g.model.nodes.length,4);assert.ok(g.model.image.startsWith('data:image/svg+xml;base64,'));assert.ok(ids.has(g.startTopicId));
  for(const section of g.sections)for(const id of section.topics){assert.ok(ids.has(id));assert.equal(c.topics.find(t=>t.id===id).chapter,ch.number);mapped.push(id);}
  if(ch.role==='chapter'){assert.ok(g.workedTopicIds.length,'Missing worked example '+ch.number);assert.equal(g.labs.length,1);assert.ok(c.topics.some(t=>t.chapter===ch.number&&t.scenario));}
 }
 assert.deepEqual(mapped,c.readingOrder);
 const css=fs.readFileSync('src/styles.css','utf8');assert.match(css,/\.linux-diagram\{[^}]*max-width:100%[^}]*object-fit:contain/);assert.match(css,/100dvh/);assert.match(css,/linux-zoom-view/);
});
test('All 39 Linux lectures cover every reading entry and preserve every original example exactly',()=>{
 assert.equal(entries.length,39);let count=0;const seen=new Set();
 for(const e of entries){const d=JSON.parse(fs.readFileSync(e.path,'utf8'));assert.equal(d.courseId,c.id);assert.equal(d.chapter,e.chapter);const ts=c.topics.filter(t=>t.chapter===e.chapter),covered=new Set(d.slides.flatMap(s=>s.topicIds));assert.deepEqual(covered,new Set(ts.map(t=>t.id)));
  const byId=new Map(ts.flatMap(t=>t.blocks).filter(b=>b.type==='code').map(b=>[b.id,b]));
  for(const s of d.slides){count++;assert.ok(!seen.has(s.id));seen.add(s.id);assert.ok(s.linuxAdmin);assert.ok(s.narration&&s.sourceRef&&s.actions.length);assert.ok(!s.code?.runAllowed);if(s.linuxCommand){assert.equal(s.linuxCommand.text,byId.get(s.linuxCommand.blockId).code);byId.delete(s.linuxCommand.blockId);}if(s.linuxModel)assert.ok(s.linuxModel.image.startsWith('data:image/svg+xml;base64,'));}
  assert.equal(byId.size,0);
  for(const t of ts)for(const p of t.reading.paragraphs)assert.ok(d.slides.some(s=>s.topicIds.includes(t.id)&&s.narration.includes(p)),'Missing narration '+t.id);
 }assert.equal(count,1615);
});
test('All 33 ordinary-user Linux lab solutions pass syntax, assertions, exact output and cleanup',()=>{
 for(let n=1;n<=33;n++){const lab=c.teaching[n].labs[0],filename=`linux-labs/ch${String(n).padStart(2,'0')}.sh`;assert.equal(fs.readFileSync(filename,'utf8'),lab.code);assert.ok(!/\bsudo\b/.test(lab.code));
  assert.equal(spawnSync('bash',['-n',filename],{encoding:'utf8'}).status,0,filename+' syntax');
  const result=spawnSync('bash',[filename],{encoding:'utf8',timeout:10000});assert.equal(result.status,0,filename+': '+result.stderr);assert.equal(result.stdout,lab.output);assert.equal(result.stderr,'');
  const starter=spawnSync('bash',[filename.replace('.sh','-starter.sh')],{encoding:'utf8',timeout:10000});assert.notEqual(starter.status,0,'Unimplemented starter must fail '+filename);
 }
});
test('Linux commands, diagram viewer and local lab interactions use the Linux renderer',()=>{
 const handlers={};const body={};const dialog={addEventListener:(type,fn)=>{handlers['dialog:'+type]=fn;},classList:{add(){},remove(){}}};let selected=c.topics.find(t=>t.chapter===16);const saved={};
 const context={course:()=>c,topic:()=>selected,progress:()=>saved,modes:[],state:{mode:'outline',lessonView:'lesson'},chapterGuide:n=>c.teaching[n??selected.chapter],escape:s=>String(s).replaceAll('&','&amp;').replaceAll('<','&lt;').replaceAll('"','&quot;'),guideTrail:()=>'',bookmarkButton:()=>'',teachingNotes:()=>'',readingFooter:()=>'',chapterMapCards:()=>'',remember(){},renderWorkspace(){},renderSlidesWorkspace(){},slidesCurrent:()=>({linuxCommand:{text:'printf test'}}),slidesCopy:s=>body.copied=s,slidesDownload:(name,text)=>body.download={name,text},document:{addEventListener:(type,fn)=>handlers[type]=fn,querySelector:()=>null},DIALOG:dialog,modal(){}};
 vm.createContext(context);vm.runInContext(fs.readFileSync('src/linux-admin.js','utf8'),context);
 for(const ch of c.chapters){selected=c.topics.find(t=>t.chapter===ch.number);context.state.lessonView='map';const html=vm.runInContext('linuxWorkspace(topic())',context);assert.ok(html.includes(ch.title.replaceAll('&','&amp;')));assert.ok(html.includes('linux-diagram'));}
 selected=c.topics.find(t=>t.chapter===16);handlers.input({target:{id:'linux-lab-editor',value:'printf edited'}});assert.equal(saved.linuxDrafts['16'],'printf edited');
 assert.equal(vm.runInContext('linuxCleanDrafts({"16":"printf saved",bad:"discard", "17":3})["16"]',context),'printf saved');
 assert.equal(Object.keys(vm.runInContext('linuxCleanDrafts({bad:"discard", "17":3})',context)).length,0);
 handlers.click({target:{closest:()=>({dataset:{action:'linux-lab-download'}})}});assert.equal(body.download.text,'printf edited');
 handlers.click({target:{closest:()=>({dataset:{action:'linux-slide-copy'}})}});assert.equal(body.copied,'printf test');
 const html=vm.runInContext('linuxSlidesWorkbench({linuxCommand:{text:"<script>alert(1)</script>",interpretation:"read",source:"book"}})',context);assert.ok(!html.includes('<script>'));assert.ok(html.includes('&lt;script>'));
 context.state.mode='code';assert.ok(vm.runInContext('linuxWorkspace(topic())',context).includes('not submitted to the C++ compiler'));
});
test('Linux installer is repeatable and preserves other lecture entries, builder edits and desktop sidebar helpers',()=>{
 const temp=fs.mkdtempSync(path.join(os.tmpdir(),'linux-installer-'));
 const files=['scripts/install-linux-admin.mjs','scripts/linux-admin-content.json.gz.b64','src/linux-admin.js','src/linux-admin.css','src/app.js','src/slides.js','src/styles.css','content/manifest.json','lectures/manifest.json','tests/modular.test.mjs','tests/book-one-expansion.test.mjs','tests/book-three.test.mjs'];
 try{
  for(const file of files){fs.mkdirSync(path.dirname(path.join(temp,file)),{recursive:true});fs.copyFileSync(file,path.join(temp,file));}
  const builder='// Existing local build extension must remain unchanged\n';fs.writeFileSync(path.join(temp,'scripts/build-slides.mjs'),builder);
  const others=JSON.parse(fs.readFileSync('lectures/manifest.json','utf8')).filter(e=>e.courseId!==c.id);
  const run=()=>spawnSync(process.execPath,[path.join(temp,'scripts/install-linux-admin.mjs')],{encoding:'utf8'});
  const first=run();assert.equal(first.status,0,first.stderr);
  const shared=['src/app.js','src/slides.js','src/styles.css','content/manifest.json','lectures/manifest.json'];const before=shared.map(f=>hash(fs.readFileSync(path.join(temp,f))));
  const second=run();assert.equal(second.status,0,second.stderr);assert.deepEqual(shared.map(f=>hash(fs.readFileSync(path.join(temp,f)))),before);
  assert.equal(fs.readFileSync(path.join(temp,'scripts/build-slides.mjs'),'utf8'),builder);
  assert.deepEqual(JSON.parse(fs.readFileSync(path.join(temp,'lectures/manifest.json'),'utf8')).filter(e=>e.courseId!==c.id),others);
  const original=fs.readFileSync('src/app.js','utf8'),installed=fs.readFileSync(path.join(temp,'src/app.js'),'utf8');if(original.includes('BEGIN DESKTOP SIDEBAR TOGGLE'))assert.ok(installed.includes('BEGIN DESKTOP SIDEBAR TOGGLE'));
 }finally{fs.rmSync(temp,{recursive:true,force:true});}
});

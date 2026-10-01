import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import crypto from 'node:crypto';
const root=new URL('../',import.meta.url),text=p=>fs.readFile(new URL(p,root),'utf8'),json=async p=>JSON.parse(await text(p));
const entries=(await json('lectures/manifest.json')).filter(x=>x.courseId==='design-patterns-cpp');
const decks=await Promise.all(entries.map(e=>json(e.path)));
test('Design Patterns has a substantive interactive lecture for every chapter, including Class 0 and capstone',async()=>{
 assert.deepEqual(entries.map(e=>e.chapter),Array.from({length:24},(_,i)=>i));
 for(const d of decks){assert.ok(d.slides.length>=26,d.title);assert.equal(d.author,'Dr. Charles Dorner');assert.equal(new Set(d.slides.map(s=>s.id)).size,d.slides.length);const a=await json(`lectures/authoring/design-patterns/ch${d.chapter}.json`);const original=[...a.opening,...a.mechanism,...a.closing];assert.ok(original.length>=(d.chapter===0?26:12));assert.ok(original.filter(s=>s.code?.runAllowed).length>=2);for(const s of original){assert.ok(s.narration.split(/\s+/).length>=70,s.title);assert.ok(s.actions.length);assert.ok(s.question||s.checks?.length);}}
});
test('Every current Design Patterns reading entry is represented and all original source hashes remain exact',async()=>{
 for(const d of decks){const c=await json(`content/design-patterns-cpp/ch${d.chapter}.json`);const covered=new Set(d.slides.flatMap(s=>s.topicIds||[]));assert.deepEqual([...covered].sort(),c.topics.map(t=>t.id).sort());}
 for(const x of await json('lectures/design-patterns-source-integrity.json'))assert.equal(crypto.createHash('sha256').update(await fs.readFile(new URL(x.file,root))).digest('hex'),x.sha256,x.file);
});
test('All focused code, UML selectors, asset paths, source line references and downloadable C++ files resolve',async()=>{
 for(const d of decks){const c=await json(`content/design-patterns-cpp/ch${d.chapter}.json`);for(const s of d.slides){
  if(s.code?.runAllowed){assert.match(s.code.text,/\bmain\s*\(/);assert.equal(await text(s.code.downloadPath),s.code.text);assert.ok(s.code.buildCommand);}
  if(s.excerpt?.lineStart){const lines=(await text(s.excerpt.filename)).split('\n');assert.equal(s.excerpt.text.trim(),lines.slice(s.excerpt.lineStart-1,s.excerpt.lineEnd).join('\n').trim(),s.id);}
  if(s.visual){await fs.access(new URL(s.visual.path,root));assert.match(await text(s.visual.path),/<svg/);assert.ok(s.visual.caption);}
  if(!s.diagram)continue;const bundle=c.diagrams[String(s.diagram.chapter)];assert.ok(bundle,s.id);const v=s.diagram.view==='focus'?bundle.focus[s.diagram.index]:s.diagram.view==='extra'?bundle.extra_overviews[s.diagram.index]:bundle.overview;assert.ok(v,s.id);
  if(s.diagram.view==='focus'){const lines=(await text('companion/'+v.file)).split('\n');assert.equal(v.code,lines.slice(v.lineStart-1,v.lineEnd).join('\n'),s.id);assert.equal(v.explain.length,v.lineEnd-v.lineStart+1);}
  else {assert.ok(v.image.$asset);await fs.access(new URL('content/'+v.image.$asset,root));assert.ok(v.explanation);}
 }}
});
test('New Slides layer preserves lazy loading, the supplied Book I lecture, and independent chapter drafts',async()=>{
 const b=await json('lectures/cpp-book-01/ch1.json');assert.equal(b.slides.length,26);assert.equal(b.slides[0].id,'b01-ch01-01');
 const code=await text('src/slides.js');assert.match(code,/await Library.json\(entry.path\)/);assert.match(code,/state.courseId\+'\/'/);assert.match(code,/slidesImport/);
 for(const d of decks)assert.ok(!code.includes(d.slides[0].narration));
});

test('Class 0 reference slides retain source code blocks and scenario alternatives',async()=>{
 const d=decks.find(d=>d.chapter===0),c=await json('content/design-patterns-cpp/ch0.json');
 for(const t of c.topics){const s=d.slides.find(s=>s.topicIds?.includes(t.id));for(const b of (t.blocks||[]).filter(b=>b.type==='code'))assert.ok(s.excerpts?.some(x=>x.text===b.code),t.id);if(t.scenario)assert.deepEqual(s.scenario,t.scenario);}
});

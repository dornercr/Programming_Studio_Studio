import test from 'node:test';import assert from 'node:assert/strict';import fs from 'node:fs';
const root=new URL('../',import.meta.url),data=JSON.parse(fs.readFileSync(new URL('src/content.json',root)));
test('Every supplied chapter has a hierarchical map and a worked discussion',()=>{
 for(const c of data.courses.filter(c=>c.kind==='textbook')){
  assert.equal(Object.keys(c.teaching).length,c.chapters.length);const allIds=new Set(c.topics.map(t=>t.id));const seen=new Set();
  for(const ch of c.chapters){const g=c.teaching[ch.number];assert.ok(allIds.has(g.startTopicId));assert.ok(g.objectives.length);assert.ok(g.sections.length>=5);
   for(const s of g.sections){assert.ok(s.title&&s.purpose);for(const id of s.topics){assert.ok(allIds.has(id));assert.ok(!seen.has(id));seen.add(id);const t=c.topics.find(t=>t.id===id);assert.equal(t.chapter,ch.number);assert.equal(t.sectionId,s.id);}}
   const w=g.workshop;for(const key of ['title','problem','code','output','pitfall','alternative','filename'])assert.ok(w[key],`${c.id}/${ch.number}/${key}`);assert.ok(w.steps.length>=3);assert.ok(w.discussion.question&&w.discussion.answer);for(const step of w.steps)assert.ok(step.action&&step.state&&step.why);
   assert.equal(w.code,fs.readFileSync(new URL(w.filename,root),'utf8'));assert.equal(w.output,fs.readFileSync(new URL(w.filename.replace('main.cpp','expected.txt'),root),'utf8'));
  }
  for(const t of c.topics.filter(t=>t.chapter>=0)){assert.ok(seen.has(t.id)||allIds.has(t.outlineParent),`unmapped ${t.id}`);if(t.outlineParent)assert.ok(c.topics.find(x=>x.id===t.outlineParent).sourceCompanions.includes(t.id));}
  assert.equal(new Set(c.readingOrder).size,c.readingOrder.length);
 }
});
test('Every original primary topic retains its source and readable explanation',()=>{
 for(const c of data.courses.filter(c=>c.teaching))for(const t of c.topics.filter(t=>t.chapter>=0&&!t.outlineParent)){assert.ok(t.reading);assert.ok(t.reading.title);assert.ok(t.reading.paragraphs.length);assert.ok(t.blocks);}
});
test('All 62 new complete demonstrations have successful compile and output records',()=>{
 const report=JSON.parse(fs.readFileSync(new URL('docs/workshop-verification.json',root)));assert.equal(report.programs.length,62);for(const p of report.programs)assert.ok(p.compiled&&p.outputMatch);
});
test('The affected race explanation includes the C++ correction',()=>{
 const c=data.courses.find(c=>c.id==='systems-programming');for(const t of c.topics.filter(t=>t.chapter===45&&t.title.includes('Lost update race')))assert.match(t.reading.correction,/undefined behavior/);
});

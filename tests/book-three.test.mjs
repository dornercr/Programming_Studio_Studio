import test from 'node:test';import assert from 'node:assert/strict';import fs from 'node:fs';
import {sourceData,sourceCoding} from '../scripts/read-source.mjs';
import {hash} from '../scripts/content-store.mjs';
const all=sourceData(),coding=sourceCoding(),book=all.courses.find(c=>c.id==='cpp-book-03');
const questions=coding.questions.filter(q=>q.courseId===book.id),worked=coding.workedPrograms.filter(w=>w.courseId===book.id);
test('Book III adds questions in every core chapter plus both executable appendices',()=>{
 assert.equal(questions.length,22);assert.equal(questions.reduce((n,q)=>n+q.tests.length,0),116);
 assert.deepEqual(questions.map(q=>q.chapter).sort((a,b)=>a-b),[...Array.from({length:20},(_,i)=>i+1),22,23]);
 for(const q of questions){assert.ok(book.chapters.some(c=>c.number===q.chapter));assert.equal(q.chapterTitle,book.chapters.find(c=>c.number===q.chapter).title);}
});
test('Book III workshops cover all 68 exact C++ listings through 66 runnable programs',()=>{
 const listings=book.series.listings.filter(l=>l.language==='cpp'),blocks=new Map(book.topics.flatMap(t=>(t.blocks||[]).map(b=>[b.id,b])));
 assert.equal(worked.length,66);assert.equal(worked.reduce((n,w)=>n+w.checks.length,0),71);
 assert.deepEqual(new Set(worked.flatMap(w=>[w.sourceId,...w.sourcePartIds])),new Set(listings.map(l=>l.id)));
 for(const w of worked){
  const original=listings.find(l=>l.id===w.sourceId);assert.equal(w.originalSource,blocks.get(original.blockId).code);
  assert.equal(w.originalSource.trimEnd(),fs.readFileSync(original.filename,'utf8').trimEnd());
  assert.ok(w.source.includes('int main('));assert.ok(w.guide.problem&&w.guide.design&&w.guide.invariant&&w.guide.trace&&w.guide.failure&&w.guide.maintenance);
  if(w.adapted)assert.ok(w.adaptationNote);else assert.equal(w.source,w.originalSource);
  for(const part of w.sourcePartIds){const source=listings.find(l=>l.id===part);assert.ok(source);assert.ok(w.source.includes('Book III source part '+part));}
  assert.equal(w.sampleOutput,w.checks[0].stdout);
 }
});
test('All original educational content and every earlier coding item remain byte-equivalent',()=>{
 const baseline=JSON.parse(fs.readFileSync('docs/book-three-baseline.json','utf8'));assert.equal(hash(all),baseline.contentHash);
 for(const key of ['questions','workedPrograms'])for(const old of baseline[key])assert.equal(hash(coding[key].find(x=>x.id===old.id)),old.hash,old.id);
});

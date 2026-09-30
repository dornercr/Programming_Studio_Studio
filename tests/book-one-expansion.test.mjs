import test from 'node:test';import assert from 'node:assert/strict';import fs from 'node:fs';
import {sourceData,sourceCoding} from '../scripts/read-source.mjs';import {bookOneExpansion as pack,withoutBookOneExpansion,withoutBookOneQuestions} from '../scripts/book-one-expansion.mjs';import {hash} from '../scripts/content-store.mjs';
const data=sourceData(),book=data.courses.find(c=>c.id==='cpp-book-01'),coding=sourceCoding(),newIds=new Set(pack.cards.map(c=>c.id));
test('Book I adds exactly 400 unique cards with balanced chapter and appendix coverage',()=>{
 const cards=book.topics.flatMap(t=>t.cards);assert.equal(cards.length,519);assert.equal(book.series.counts.cards,519);assert.equal(new Set(cards.map(c=>c.id)).size,519);
 for(let n=1;n<=24;n++){const added=book.topics.filter(t=>t.chapter===n).flatMap(t=>t.cards).filter(c=>newIds.has(c.id));assert.equal(added.length,n<=20?18:10,'chapter '+n);}
 for(const {topicId,chapter,...card} of pack.cards){const t=book.topics.find(t=>t.id===topicId);assert.equal(t.chapter,chapter);assert.deepEqual(t.cards.find(c=>c.id===card.id),card);assert.ok(card.question.length>15&&card.answer.length>30);}
});
test('Every added diagram matches its actual delivered C++ and source lines',()=>{
 assert.equal(Object.keys(book.diagrams).length,24);assert.equal(Object.values(book.diagrams).reduce((n,d)=>n+1+d.extra_overviews.length,0),25);
 for(const d of pack.diagrams){const source=fs.readFileSync(d.source,'utf8'),lines=source.split('\n'),flat=x=>x.replace(/\s/g,'');for(const f of d.focus){assert.equal(f.code,lines.slice(f.lineStart-1,f.lineEnd).join('\n'));assert.equal(f.explain.length,f.lineEnd-f.lineStart+1);for(const k of ['what','where','why','invariant','risk'])assert.ok(f[k].length>20);}
  const v=d.overview,ids=new Set(v.nodes.map(n=>n.id));for(const e of v.edges)assert.ok(ids.has(e.from)&&ids.has(e.to));for(const n of v.nodes)for(const code of n.code)if(!code.trim().startsWith('//'))assert.ok(flat(source).includes(flat(code)),d.expansionId+': '+code);
  assert.ok(fs.existsSync('content/'+v.image.$asset));assert.ok(fs.existsSync('diagrams/book_one_expansion/ch'+String(d.chapter).padStart(2,'0')+'.dot'));
 }
});
test('Every new question has a full contract, guide, hints, sample and boundary checks',()=>{
 const qs=coding.questions.filter(q=>q.id.startsWith('b1x-'));assert.equal(qs.length,24);assert.equal(qs.reduce((n,q)=>n+q.tests.length,0),128);assert.equal(coding.questions.filter(q=>q.courseId===book.id).length,47);
 for(const q of qs){assert.deepEqual(q,pack.questions.find(x=>x.id===q.id));for(const key of ['problem','design','invariant','trace','failure','maintenance'])assert.ok(q.guide[key].length>20);assert.ok(q.tests.length>=4);for(const name of ['starter','solution'])assert.equal(fs.readFileSync(`coding_lab/book_one_expansion/ch${String(q.chapter).padStart(2,'0')}/${name}.cpp`,'utf8'),q[name]+'\n'+q.driver);}
});
test('Removing only these additions reconstructs the exact historical content and prior coding catalog',()=>{
 const baseline=JSON.parse(fs.readFileSync('docs/book-three-baseline.json','utf8'));assert.equal(hash(withoutBookOneExpansion(data)),baseline.contentHash);
 const prior=withoutBookOneQuestions(coding);for(const key of ['questions','workedPrograms'])for(const old of baseline[key])assert.equal(hash(prior[key].find(x=>x.id===old.id)),old.hash,old.id);
 assert.equal(withoutBookOneExpansion(data).courses.find(c=>c.id===book.id).topics.flatMap(t=>t.cards).length,119);
});

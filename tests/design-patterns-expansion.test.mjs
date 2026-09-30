import crypto from 'node:crypto';
import test from 'node:test';import assert from 'node:assert/strict';import fs from 'node:fs';import {sourceData,sourceCoding} from '../scripts/read-source.mjs';import {designPatternsExpansion as pack,withoutDesignPatternsExpansion,withoutDesignPatternsCoding} from '../scripts/design-patterns-expansion.mjs';import {hash} from '../scripts/content-store.mjs';
const all=sourceData(),coding=sourceCoding(),c=all.courses.find(x=>x.id===pack.courseId),prior=withoutDesignPatternsExpansion(all).courses.find(x=>x.id===pack.courseId);
test('Design Patterns retains every original identity and adds 400 contextual cards',()=>{
 assert.equal(c.topics.length,539);assert.equal(c.chapters.length,24);assert.deepEqual(c.topics.map(t=>t.id),prior.topics.map(t=>t.id));const cards=c.topics.flatMap(t=>t.cards);assert.equal(cards.length,1009);assert.equal(prior.topics.flatMap(t=>t.cards).length,609);assert.equal(new Set(cards.map(x=>x.id)).size,1009);const labs=new Set(coding.questions.map(q=>q.id));
 for(const {chapter,topicId,...card} of pack.cards){const t=c.topics.find(t=>t.id===topicId);assert.equal(t.chapter,chapter);assert.deepEqual(t.cards.find(x=>x.id===card.id),card);assert.ok(labs.has(card.labId));assert.ok(card.answer.length>30);}
 for(let n=1;n<=23;n++)assert.equal(pack.cards.filter(x=>x.chapter===n).length,n===23?26:17);
 const fronts=pack.cards.map(x=>x.question+'\n'+(x.code||''));assert.equal(new Set(fronts).size,400);
});
test('22 additional diagrams quote exact, consecutive original source lines and preserve old views',()=>{
 assert.equal(Object.values(c.diagrams).reduce((n,d)=>n+1+d.extra_overviews.length,0),47);assert.equal(Object.values(prior.diagrams).reduce((n,d)=>n+1+d.extra_overviews.length,0),25);
 for(const d of pack.diagrams){const lines=fs.readFileSync(d.source,'utf8').split('\n');let last=null;for(const node of d.overview.nodes){assert.equal(node.code.join('\n'),lines.slice(node.lineStart-1,node.lineEnd).join('\n'));if(last)assert.equal(node.lineStart,last+1);last=node.lineEnd;for(const k of ['what','where','why','invariant','risk','file'])assert.ok(node.theory[k]);assert.equal(node.theory.file,d.source);}
 assert.ok(fs.existsSync('content/'+d.overview.image.$asset));assert.ok(fs.existsSync(`diagrams/design_patterns_expansion/ch${String(d.chapter).padStart(2,'0')}.dot`));}
});
test('50 questions and 69 worked programs retain complete C++ and exact recorded behavior',()=>{
 const qs=coding.questions.filter(q=>q.courseId===c.id),ws=coding.workedPrograms.filter(w=>w.courseId===c.id);assert.equal(qs.length,50);assert.equal(qs.reduce((n,q)=>n+q.tests.length,0),388);assert.equal(ws.length,69);
 for(const q of pack.questions){assert.deepEqual(qs.find(x=>x.id===q.id),q);assert.equal(q.tests.length,8);assert.notEqual(q.starter,q.solution);for(const key of ['problem','design','invariant','trace','failure','maintenance'])assert.ok(q.guide[key]);const part=q.id.at(-1);for(const kind of ['starter','solution'])assert.equal(fs.readFileSync(`coding_lab/design_patterns_expansion/ch${String(q.chapter).padStart(2,'0')}/${part}_${kind}.cpp`,'utf8'),q[kind]+'\n'+q.driver);}
 for(const w of ws){assert.equal(w.source,fs.readFileSync(w.sourceFilename,'utf8'));assert.equal(w.originalSource,w.source);assert.equal(w.sampleOutput,fs.readFileSync(w.sourceFilename.replace('main.cpp','expected.txt'),'utf8'));assert.equal(w.checks[0].stdout,w.sampleOutput);}
});
test('Reviewed editorial ledger restores exact approved source and refuses untracked drift',()=>{
 const baseline=JSON.parse(fs.readFileSync('docs/pre-series-approved.json','utf8'));const stable=x=>Array.isArray(x)?'['+x.map(stable).join(',')+']':x&&typeof x==='object'?'{'+Object.keys(x).sort().map(k=>JSON.stringify(k)+':'+stable(x[k])).join(',')+'}':JSON.stringify(x);assert.equal(crypto.createHash('sha256').update(stable(prior)).digest('hex'),baseline[c.id]);const before=withoutDesignPatternsCoding(coding),legacy=JSON.parse(fs.readFileSync('docs/book-three-baseline.json','utf8'));for(const old of legacy.questions.filter(x=>before.questions.find(q=>q.id===x.id)?.courseId===c.id))assert.equal(hash(before.questions.find(q=>q.id===old.id)),old.hash);
 assert.equal(before.questions.filter(q=>q.courseId===c.id).length,3);assert.equal(before.workedPrograms.filter(w=>w.courseId===c.id).length,0);
 const bad=structuredClone(all);bad.courses.find(x=>x.id===c.id).topics.find(t=>t.id==='C01.01').summary='untracked loss';assert.throws(()=>withoutDesignPatternsExpansion(bad),/editorial drift/);
 for(const t of c.topics.filter(t=>t.chapter>0&&t.reading)){assert.ok(!t.reading.paragraphs.includes('Open Scenarios to select an answer and compare the reasoning for all four choices.'));if(t.title==='Pattern overview')assert.ok(!/attached|EPUB/.test(t.summary));}
});

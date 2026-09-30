import {sourceData,sourceCoding} from '../scripts/read-source.mjs';
import test from 'node:test';import assert from 'node:assert/strict';import fs from 'node:fs';import crypto from 'node:crypto';
const root=new URL('../',import.meta.url),data=sourceData(),c=data.courses.find(c=>c.id==='systems-programming');
test('Every Systems chapter has all three runnable files, exercises, glossary and diagrams',()=>{
 assert.equal(Object.keys(c.examples).length,61);assert.equal(Object.keys(c.diagrams).length,61);assert.equal(c.glossary.length,122);
 for(let n=1;n<=61;n++){
  const e=c.examples[n];for(const k of ['examples','exercises','solutions']){assert.equal(e[k].code,fs.readFileSync(new URL(e[k].filename,root),'utf8'));const expected=e[k].filename.replace(/main.cpp$/,'expected.txt').replace(/(starter|solution).cpp$/,'$1_expected.txt');assert.equal(e[k].output,fs.readFileSync(new URL(expected,root),'utf8'));}
  assert.notEqual(e.exercises.code,e.solutions.code);assert.ok(e.lab.checks.length>=3);
  const ts=c.topics.filter(t=>t.chapter===n&&t.exercise);assert.equal(ts.length,6);for(const t of ts){assert.ok(t.exercise.hint.length>40);assert.ok(t.exercise.answer.length>100);assert.equal(t.blocks.filter(b=>b.type==='reveal').length,2);}
  const d=c.diagrams[n];for(const f of d.focus){const lines=fs.readFileSync(new URL(f.file,root),'utf8').split('\n');assert.equal(f.code,lines.slice(f.lineStart-1,f.lineEnd).join('\n'));assert.equal(f.explain.length,f.lineEnd-f.lineStart+1);for(const key of ['what','where','why','invariant','risk'])assert.ok(f[key].length>20);}
  for(const v of [d.overview,...d.extra_overviews]){assert.ok(v.image.startsWith('data:image/svg+xml;base64,'));const ids=new Set(v.nodes.map(n=>n.id));for(const edge of v.edges){assert.ok(ids.has(edge.from));assert.ok(ids.has(edge.to));}for(const node of v.nodes)for(const line of node.code){if(line.trim().startsWith('//'))continue;assert.ok([e.examples.code,e.solutions.code].some(s=>s.split('\n').some(x=>x.trim()===line.trim())),`Unmatched diagram line in ${n}: ${line}`);}}
 }
});
test('Every Systems lab compiled, ran and matched its output',()=>{const r=JSON.parse(fs.readFileSync(new URL('docs/systems-lab-verification.json',root)));assert.equal(r.programs.length,122);for(const p of r.programs){assert.ok(p.compiled&&p.outputMatch,p.file);assert.ok(!p.error);assert.equal(crypto.createHash('sha256').update(fs.readFileSync(new URL(p.file,root))).digest('hex'),p.sha256);}});
test('Approved Design Patterns curriculum is unchanged',()=>{const before=fs.readFileSync(new URL('docs/design-patterns-approved.sha256',root),'utf8').trim();const stable=x=>Array.isArray(x)?'['+x.map(stable).join(',')+']':x&&typeof x==='object'?'{'+Object.keys(x).sort().map(k=>JSON.stringify(k)+':'+stable(x[k])).join(',')+'}':JSON.stringify(x);const actual=crypto.createHash('sha256').update(stable(data.courses[0])).digest('hex');assert.equal(actual,before);});

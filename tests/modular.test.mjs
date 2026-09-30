import {withoutBookOneExpansion,withoutBookOneQuestions} from '../scripts/book-one-expansion.mjs';
import test from 'node:test';import assert from 'node:assert/strict';import fs from 'node:fs/promises';import path from 'node:path';import {gunzipSync} from 'node:zlib';
import {assemble,codingSource,readJSON,expandAssets,canonical,hash,counts} from '../scripts/content-store.mjs';
const before=await readJSON('docs/migration/baseline.json'),original=await assemble(),coding=await codingSource(),catalog=await readJSON('dist/data/catalog.json');
const rebuilt={schemaVersion:catalog.schemaVersion,courses:[]},labs={version:coding.version,standard:coding.standard,questions:[],workedPrograms:[]};
for(const entry of catalog.courses){const c=await readJSON('dist/'+entry.catalog);for(const file of Object.values(c._chapters)){const chunk=await readJSON('dist/'+file),byId=new Map(chunk.topics.map(t=>[t.id,t]));c.topics=c.topics.map(t=>byId.get(t.id)||t);for(const k of ['teaching','examples','diagrams','lectures'])if(chunk[k])Object.assign(c[k],chunk[k]);if(chunk.listings){const m=new Map(chunk.listings.map(l=>[l.id,l]));c.series.listings=c.series.listings.map(l=>m.get(l.id)||l);}}const lab=await readJSON('dist/'+c._coding);labs.questions.push(...lab.questions);labs.workedPrograms.push(...lab.workedPrograms);for(const k of Object.keys(c))if(k.startsWith('_'))delete c[k];rebuilt.courses.push(c);}
for(const key of ['questions','workedPrograms']){const order=new Map(coding[key].map((q,i)=>[q.id,i]));labs[key].sort((a,b)=>order.get(a.id)-order.get(b.id));}
const restored=await expandAssets(rebuilt,'dist');
test('Every source field survives migration and generated chapter hydration exactly',async()=>{
 assert.equal(hash(withoutBookOneExpansion(original)),before.contentHash);
 const editorial=await readJSON('docs/book-three-baseline.json');
 const prior={...coding};for(const key of ['questions','workedPrograms']){
  const ids=new Set(editorial[key].map(x=>x.id));prior[key]=coding[key].filter(x=>ids.has(x.id));
  assert.equal(prior[key].length,editorial[key].length,'A previous coding item was removed');
  for(const entry of editorial[key])assert.equal(hash(prior[key].find(x=>x.id===entry.id)),entry.hash,'Previous coding item changed: '+entry.id);
 }
 assert.equal(hash(prior),before.codingHash,'Original coding catalog must remain exactly reconstructible');
 assert.equal(canonical(restored),canonical(original));assert.equal(canonical(labs),canonical(coding));
 assert.deepEqual(counts(withoutBookOneExpansion(restored),prior),before.counts);
 assert.equal(withoutBookOneQuestions(coding).questions.length-prior.questions.length,19);
 assert.equal(coding.workedPrograms.length-prior.workedPrograms.length,66);
});
test('Every catalog chapter, section, topic, listing and search result resolves',async()=>{for(const c of rebuilt.courses){const topics=new Map(c.topics.map(t=>[t.id,t]));for(const ch of c.chapters||[])assert.ok(c.topics.some(t=>t.chapter===ch.number),`${c.id}/${ch.number}`);for(const g of Object.values(c.teaching||{})){assert.ok(topics.has(g.startTopicId));for(const s of g.sections)for(const id of s.topics)assert.ok(topics.has(id),id);}const blocks=new Set(c.topics.flatMap(t=>(t.blocks||[]).map(b=>b.id)));for(const t of Object.values(c.series?.links||{})){assert.ok(topics.has(t.topicId));assert.ok(blocks.has(t.blockId));}for(const l of c.series?.listings||[]){assert.ok(topics.has(l.topicId));assert.ok(blocks.has(l.blockId));await fs.access('dist/resources/'+l.filename);}
 const entry=catalog.courses.find(x=>x.id===c.id),meta=await readJSON('dist/'+entry.catalog);const index=Object.assign({},...await Promise.all(meta._search.map(async f=>JSON.parse(gunzipSync(await fs.readFile('dist/'+f))))));assert.deepEqual(Object.keys(index).sort(),[...topics.keys()].sort());for(const t of c.topics){const expected=`${JSON.stringify(t.blocks||[])} ${JSON.stringify(t.reading||{})} ${t.id} ${t.title} ${t.chapterTitle||''} ${(t.tasks||[]).join(' ')} ${t.summary} ${t.concepts.map(c=>`${c.term} ${c.definition}`).join(' ')}`.toLocaleLowerCase();assert.equal(index[t.id],expected);}}
});
test('Every emitted asset matches its hash; deployment paths are relative and bounded',async()=>{const m=await readJSON('dist/integrity.json');for(const f of m.files){assert.ok(!f.file.startsWith('/')&&!f.file.includes('..'));const b=await fs.readFile('dist/'+f.file);assert.equal(hash(b),f.sha256,f.file);assert.ok(b.length<95000000,f.file);if(/\/ch.*\.json$/.test(f.file))assert.ok(b.length<5000000,f.file);}assert.ok((await fs.stat('dist/index.html')).size<100000);assert.ok((await fs.stat('dist/app.js')).size<2000000);assert.ok((await fs.stat('dist/data/catalog.json')).size<1000000);});

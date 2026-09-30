import assert from 'node:assert/strict';import fs from 'node:fs/promises';
import {assemble,codingSource,readJSON,expandAssets,canonical,hash,counts} from './content-store.mjs';
const source=await assemble(),coding=await codingSource(),catalog=await readJSON('dist/data/catalog.json'),output={schemaVersion:catalog.schemaVersion,courses:[]};
for(const e of catalog.courses){
 const c=await readJSON('dist/'+e.catalog),index=new Map(c.topics.map((t,i)=>[t.id,i]));assert.equal(index.size,c.topics.length,e.id+' duplicate topic');
 for(const [n,f]of Object.entries(c._chapters)){const chunk=await readJSON('dist/'+f);assert.ok((await fs.stat('dist/'+f)).size<5000000,`${f} exceeds 5 MB: split this chapter before publishing.`);for(const t of chunk.topics){assert.equal(String(t.chapter),n);assert.ok(index.has(t.id));c.topics[index.get(t.id)]=t;}for(const k of ['teaching','examples','diagrams','lectures'])if(chunk[k])Object.assign(c[k],chunk[k]);if(chunk.listings){const ls=new Map(chunk.listings.map(l=>[l.id,l]));c.series.listings=c.series.listings.map(l=>ls.get(l.id)||l);}}
 const topics=new Set(c.topics.map(t=>t.id)),blocks=new Set(c.topics.flatMap(t=>(t.blocks||[]).map(b=>b.id)));
 for(const ch of c.chapters||[])assert.ok(c.topics.some(t=>t.chapter===ch.number),e.id+'/'+ch.number);
 for(const g of Object.values(c.teaching||{})){assert.ok(topics.has(g.startTopicId));for(const s of g.sections)for(const id of s.topics)assert.ok(topics.has(id),id);}
 for(const t of Object.values(c.series?.links||{})){assert.ok(topics.has(t.topicId),t.topicId);assert.ok(blocks.has(t.blockId),t.blockId);}
 for(const l of c.series?.listings||[]){assert.ok(topics.has(l.topicId));assert.ok(blocks.has(l.blockId));await fs.access('dist/resources/'+l.filename);}
 const lab=await readJSON('dist/'+c._coding);for(const k of ['questions','workedPrograms'])assert.equal(canonical(lab[k]),canonical(coding[k].filter(q=>q.courseId===c.id)));
 for(const k of Object.keys(c))if(k.startsWith('_'))delete c[k];output.courses.push(c);
}
assert.equal(canonical(await expandAssets(output,'dist')),canonical(source),'Source-to-output content mismatch');
console.log('Validated every content field, course/chapter/section reference, listing, asset and lab against modular source.');

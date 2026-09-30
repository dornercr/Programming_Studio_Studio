// One-time, lossless import of an existing monolithic source. Never paraphrases content.
import fs from 'node:fs/promises';
import path from 'node:path';
import {readJSON,writeJSON,hash,counts,assemble,codingSource,canonical} from './content-store.mjs';
const input=process.argv[2]||'src/content.json',codingInput=process.argv[3]||'src/coding-content.json',root=process.argv[4]||'content';
const original=await readJSON(input),coding=await readJSON(codingInput);const data=structuredClone(original);
const manifest={format:1,root:{...data},courses:[],codingRoot:{...coding},coding:[],codingOrder:{questions:coding.questions.map(x=>x.id),workedPrograms:coding.workedPrograms.map(x=>x.id)}};delete manifest.root.courses;delete manifest.codingRoot.questions;delete manifest.codingRoot.workedPrograms;
async function externalize(x,key=''){
 if(typeof x==='string'&&(x.startsWith('data:image/')||key==='base64'||key==='presentationBase64')){
  const i=x.startsWith('data:')?x.indexOf(',')+1:0,prefix=x.slice(0,i),raw=Buffer.from(x.slice(i),'base64');
  if(raw.toString('base64')!==x.slice(i))throw Error('Noncanonical asset encoding: '+key);
  const ext=prefix.includes('svg')?'svg':prefix.includes('png')?'png':key==='presentationBase64'?'pptx':raw.subarray(0,4).toString()==='%PDF'?'pdf':key==='base64'?'epub':'bin';
  const name=`assets/${hash(raw)}.${ext}`;await fs.mkdir(path.join(root,'assets'),{recursive:true});await fs.writeFile(path.join(root,name),raw);return {$asset:name,prefix};
 }
 if(Array.isArray(x))return Promise.all(x.map(v=>externalize(v)));
 if(x&&typeof x==='object')return Object.fromEntries(await Promise.all(Object.entries(x).map(async([k,v])=>[k,await externalize(v,k)])));
 return x;
}
for(const raw of data.courses){
 const c=await externalize(raw),base={...c,topics:[]},entry={id:c.id,base:`${c.id}/source.json`,chunks:[],topicOrder:c.topics.map(t=>t.id),listingOrder:c.series?.listings.map(l=>l.id)||[]};
 for(const k of ['teaching','examples','diagrams','lectures'])if(k in c)base[k]={};if(c.series)base.series={...c.series,listings:[]};
 const groups=new Set(c.topics.map(t=>String(t.chapter)));for(const k of ['teaching','examples','diagrams','lectures'])for(const n of Object.keys(c[k]||{}))groups.add(n);
 for(const n of groups){const chunk={topics:c.topics.filter(t=>String(t.chapter)===n)};for(const k of ['teaching','examples','diagrams','lectures'])if(c[k]&&n in c[k])chunk[k]={[n]:c[k][n]};if(c.series)chunk.listings=c.series.listings.filter(l=>String(l.chapter)===n);const f=`${c.id}/ch${n}.json`;entry.chunks.push(f);await writeJSON(path.join(root,f),chunk);}
 await writeJSON(path.join(root,entry.base),base);manifest.courses.push(entry);
 const lab={questions:coding.questions.filter(q=>q.courseId===c.id),workedPrograms:coding.workedPrograms.filter(q=>q.courseId===c.id)};const f=`${c.id}/coding.json`;await writeJSON(path.join(root,f),lab);manifest.coding.push(f);
}
await writeJSON(path.join(root,'manifest.json'),manifest);
const restored=await assemble(root),restoredCoding=await codingSource(root);
if(canonical(restored)!==canonical(original)||canonical(restoredCoding)!==canonical(coding))throw Error('Lossless migration failed');
const baseline={contentHash:hash(original),codingHash:hash(coding),counts:counts(original,coding),sourceBytes:(await fs.stat(input)).size,indexBytes:(await fs.stat('dist/index.html')).size,distBytes:(await fs.stat('dist/index.html')).size};
await writeJSON('docs/migration/baseline.json',baseline);console.log(baseline);

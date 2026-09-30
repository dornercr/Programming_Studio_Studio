import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
export const canonical=x=>JSON.stringify(x,(_,v)=>v&&typeof v==='object'&&!Array.isArray(v)?Object.fromEntries(Object.keys(v).sort().map(k=>[k,v[k]])):v);
export const hash=x=>crypto.createHash('sha256').update(typeof x==='string'||Buffer.isBuffer(x)?x:canonical(x)).digest('hex');
export const readJSON=async p=>JSON.parse(await fs.readFile(p,'utf8'));
export async function writeJSON(p,x){await fs.mkdir(path.dirname(p),{recursive:true});await fs.writeFile(p,JSON.stringify(x));}
export async function expandAssets(x,root){
 if(x?.$asset){const b=await fs.readFile(path.join(root,x.$asset));return x.prefix+b.toString('base64');}
 if(Array.isArray(x))return Promise.all(x.map(v=>expandAssets(v,root)));
 if(x&&typeof x==='object')return Object.fromEntries(await Promise.all(Object.entries(x).map(async([k,v])=>[k,await expandAssets(v,root)])));
 return x;
}
export async function assemble(root='content',assets=true){
 const manifest=await readJSON(path.join(root,'manifest.json'));const result={...manifest.root,courses:[]};
 for(const entry of manifest.courses){
  const c=await readJSON(path.join(root,entry.base));c.topics=[];
  for(const f of entry.chunks){const chunk=await readJSON(path.join(root,f));c.topics.push(...chunk.topics);for(const k of ['teaching','examples','diagrams','lectures'])if(chunk[k])Object.assign(c[k],chunk[k]);if(chunk.listings)c.series.listings.push(...chunk.listings);}
  const order=new Map(entry.topicOrder.map((id,i)=>[id,i]));c.topics.sort((a,b)=>order.get(a.id)-order.get(b.id));
  if(c.series){const order=new Map(entry.listingOrder.map((id,i)=>[id,i]));c.series.listings.sort((a,b)=>order.get(a.id)-order.get(b.id));}
  result.courses.push(c);
 }
 return assets?expandAssets(result,root):result;
}
export async function codingSource(root='content'){const m=await readJSON(path.join(root,'manifest.json'));const c={...m.codingRoot,questions:[],workedPrograms:[]};for(const f of m.coding){const x=await readJSON(path.join(root,f));c.questions.push(...x.questions);c.workedPrograms.push(...x.workedPrograms);}for(const k of ['questions','workedPrograms']){const ids=new Map(m.codingOrder[k].map((id,i)=>[id,i]));c[k].sort((a,b)=>ids.get(a.id)-ids.get(b.id));}return c;}
export function counts(data,coding){
 const cs=data.courses.filter(c=>c.id!=='my-material'),ts=cs.flatMap(c=>c.topics);let blocks=0,images=0;function walk(x){if(!x||typeof x!=='object')return;if(x.type==='code'||x.kind==='code')blocks++;for(const[k,v]of Object.entries(x)){if(k==='image'&&v)images++;walk(v);}}walk(data);
 return {courses:cs.length,books:cs.filter(c=>c.kind==='textbook').length,cppBooks:cs.filter(c=>c.series).length,chapters:cs.reduce((n,c)=>n+(c.chapters?.length||0),0),topicChapterGroups:cs.reduce((n,c)=>n+new Set(c.topics.map(t=>t.chapter)).size,0),lessons:ts.length,readingEntries:cs.reduce((n,c)=>n+(c.readingOrder?.length||0),0),sections:cs.reduce((n,c)=>n+Object.values(c.teaching||{}).reduce((s,g)=>s+(g.sections?.length||0),0),0),flashcards:ts.reduce((n,t)=>n+t.cards.length,0),practice:ts.reduce((n,t)=>n+(t.practice?.length||0)+(t.scenario?1:0),0),sourceListings:cs.reduce((n,c)=>n+(c.series?.listings.length||0),0),codeBlocks:blocks,exampleChapters:cs.reduce((n,c)=>n+Object.keys(c.examples||{}).length,0),codingQuestions:coding.questions.length,workedPrograms:coding.workedPrograms.length,publicCodingChecks:coding.questions.reduce((n,q)=>n+q.tests.length,0),diagramChapters:cs.reduce((n,c)=>n+Object.keys(c.diagrams||{}).length,0),diagramImages:images};
}

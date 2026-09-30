import fs from 'node:fs/promises';
import path from 'node:path';
import {gzipSync} from 'node:zlib';
import {assemble,codingSource,readJSON,writeJSON,hash,counts} from './content-store.mjs';
const root=path.resolve(import.meta.dirname,'..');process.chdir(root);
const data=await assemble('content',false),coding=await codingSource(),manifest=await readJSON('content/manifest.json');
await fs.rm('dist',{recursive:true,force:true});await fs.mkdir('dist',{recursive:true});
await fs.cp('content/assets','dist/assets',{recursive:true});
const catalog={schemaVersion:data.schemaVersion,courses:[]};
const pick=(x,keys)=>Object.fromEntries(keys.filter(k=>k in x).map(k=>[k,x[k]]));
for(const c of data.courses){
 const dir=`data/${c.id}`,entry=manifest.courses.find(e=>e.id===c.id);
 const master=pick(c,['id','kind','title','subtitle','description','chapters','domains']);master.topics=[];master.glossary=c.glossary?.length?[{}]:[];if(c.series)master.series=pick(c.series,['roman']);master.catalog=`${dir}/catalog.json`;catalog.courses.push(master);
 const meta=structuredClone(c);
 meta.topics=c.topics.map(t=>({...pick(t,['id','chapter','chapterTitle','domain','domains','title','tasks','sectionId','outlineParent','sourceCompanions','kind','seriesKind','slide']),summary:'',concepts:[],cards:t.cards.map(x=>({id:x.id})),...(t.scenario?{scenario:{id:t.scenario.id}}:{}),...(t.practice?{practice:t.practice.map(x=>({id:x.id}))}:{}),...(t.reading?{reading:pick(t.reading,['title'])}:{})}));
 for(const k of ['examples','lectures','diagrams'])if(c[k])meta[k]=Object.fromEntries(Object.entries(c[k]).map(([n,x])=>[n,{...pick(x,['slideCount','title']),...(x.extra_overviews?{extra_overviews:x.extra_overviews.map(()=>({}))}:{})}]));
 if(c.teaching)meta.teaching=Object.fromEntries(Object.entries(c.teaching).map(([n,g])=>[n,{...pick(g,['chapter','title','startTopicId','firstLessonId']),sections:g.sections.map(s=>pick(s,['id','number','title','topics'])),workshop:{problem:g.workshop?.problem||''}}]));
 if(c.series)meta.series.listings=c.series.listings.map(l=>pick(l,['id','topicId','chapter','blockId','language','title','filename','kind','completeCandidate']));
 meta._chapters={};
 for(const f of entry.chunks){const dest=`${dir}/${path.basename(f)}`;await fs.cp(`content/${f}`,`dist/${dest}`,{recursive:true,force:true}).catch(async e=>{if(e.code!=='ENOENT')throw e;await fs.mkdir(`dist/${dir}`,{recursive:true});await fs.copyFile(`content/${f}`,`dist/${dest}`);});const chunk=await readJSON(`content/${f}`);const n=path.basename(f).slice(2,-5);meta._chapters[n]=dest;}
 const lab=await readJSON(`content/${c.id}/coding.json`);meta._coding=`${dir}/coding.json`;meta._lab={questions:lab.questions.map(q=>pick(q,['id','courseId','chapter','title','chapterTitle'])),workedPrograms:lab.workedPrograms.map(q=>pick(q,['id','courseId','chapter','title','chapterTitle','sourceId','sourcePartIds','sourceVariant']))};await writeJSON(`dist/${meta._coding}`,lab);
 // Separate search-only text preserves the original substring search, including code.
 // Compressed shards are fetched only for a search in the selected book.
 let shard={},bytes=0,i=0;meta._search=[];
 async function flush(){if(!Object.keys(shard).length)return;const file=`${dir}/search-${i++}.json.gz`;await fs.writeFile(`dist/${file}`,gzipSync(JSON.stringify(shard),{level:9}));meta._search.push(file);shard={};bytes=0;}
 for(const t of c.topics){const text=`${JSON.stringify(t.blocks||[])} ${JSON.stringify(t.reading||{})} ${t.id} ${t.title} ${t.chapterTitle||''} ${(t.tasks||[]).join(' ')} ${t.summary} ${t.concepts.map(c=>`${c.term} ${c.definition}`).join(' ')}`.toLocaleLowerCase();if(bytes+Buffer.byteLength(text)>400000)await flush();shard[t.id]=text;bytes+=Buffer.byteLength(text);}await flush();
 await writeJSON(`dist/${master.catalog}`,meta);
}
await writeJSON('dist/data/catalog.json',catalog);
const read=p=>fs.readFile(p,'utf8');
let app=await read('src/app.js');for(const [tag,file]of [['TEACHING_JS','teaching.js'],['SERIES_JS','series.js'],['CODING_CORE','coding-core.mjs'],['CODING_JS','coding.js']])app=app.replace(`/*__${tag}__*/`,()=>'' /* filled below */+ '');
app=await read('src/app.js');for(const [tag,file]of [['TEACHING_JS','teaching.js'],['SERIES_JS','series.js'],['CODING_CORE','coding-core.mjs'],['CODING_JS','coding.js']]){let text=await read(`src/${file}`);if(tag==='CODING_CORE')text=text.replace(/^export /gm,'');app=app.replace(`/*__${tag}__*/`,()=>text);}
await fs.writeFile('dist/app.js',app);await fs.copyFile('src/loader.js','dist/loader.js');
await fs.writeFile('dist/styles.css',(await read('vendor/tailwind.css'))+'\n'+await read('src/styles.css'));
let html=await read('src/index.html');html=html.replace(/  <style>[\s\S]*?<\/style>\n  <style>[\s\S]*?<\/style>/,'  <link rel="stylesheet" href="./styles.css">').replace(/  <script id="study-content"[\s\S]*?<script>\/\*__APP_JS__\*\/<\/script>/,'  <script type="module" src="./app.js"></script>');await fs.writeFile('dist/index.html',html);await fs.writeFile('dist/.nojekyll','');
// Original files remain separate downloadable static resources with their original names.
for(const dir of ['companion','cpp_series','coding_lab','systems_examples','systems_labs','systems_source','diagrams','teaching'])await fs.cp(dir,`dist/resources/${dir}`,{recursive:true,filter:s=>!/(^|\/)(build|__pycache__)(\/|$)/.test(s)});
const files=[];async function walk(p){for(const e of await fs.readdir(p,{withFileTypes:true})){const f=path.join(p,e.name);if(e.isDirectory())await walk(f);else files.push({file:path.relative('dist',f),bytes:(await fs.stat(f)).size,sha256:hash(await fs.readFile(f))});}}await walk('dist');
await writeJSON('dist/integrity.json',{format:1,sourceHash:hash(await assemble()),codingHash:hash(coding),counts:counts(data,coding),files});
console.log(`Built modular library: ${files.length} files, ${files.reduce((n,f)=>n+f.bytes,0)} bytes`);
await import('./validate-build.mjs');

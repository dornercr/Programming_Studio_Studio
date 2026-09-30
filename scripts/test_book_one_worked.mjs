import {sourceData,sourceCoding} from './read-source.mjs';
import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath} from 'node:url';import {execFileSync,spawnSync} from 'node:child_process';import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..'),catalog=sourceCoding();
const worked=catalog.workedPrograms.filter(x=>x.courseId==='cpp-book-01');assert.equal(worked.length,86);
const source=sourceData().courses.find(c=>c.id==='cpp-book-01');const cpp=new Set(source.series.listings.filter(e=>e.language==='cpp').map(e=>e.id)),covered=new Set(worked.flatMap(w=>[w.sourceId,...w.sourcePartIds]));assert.deepEqual([...covered].sort(),[...cpp].sort());
const build=path.join(root,'coding_lab/build');await fs.mkdir(build,{recursive:true});const results=[];
for(const x of worked){
 const filename=path.join(root,'coding_lab/book_01_worked',x.sourceId,'main.cpp'),original=path.join(root,'coding_lab/book_01_worked',x.sourceId,'original.cpp'),binary=path.join(build,'worked-'+x.sourceId);
 assert.equal(await fs.readFile(filename,'utf8'),x.source.endsWith('\n')?x.source:x.source+'\n');assert.equal(await fs.readFile(original,'utf8'),x.originalSource.endsWith('\n')?x.originalSource:x.originalSource+'\n');
 const compile=spawnSync('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',filename,'-o',binary],{encoding:'utf8',timeout:30000});assert.equal(compile.status,0,`${x.sourceId} compile:\n${compile.stderr}`);
 let statuses=[];for(const c of x.checks){const run=spawnSync(binary,[],{input:c.input,encoding:'utf8',timeout:5000});assert.ok(!run.error,`${x.sourceId} ${c.label}: ${run.error}`);assert.equal(run.status,c.exitCode,`${x.sourceId} ${c.label} exit`);assert.equal(run.stdout,c.stdout,`${x.sourceId} ${c.label} stdout`);assert.equal(run.stderr,c.stderr,`${x.sourceId} ${c.label} stderr`);statuses.push(c.label);}
 results.push({id:x.sourceId,chapter:x.chapter,adapted:x.adapted,cases:statuses});console.log(x.sourceId+' CH'+x.chapter+' '+(x.adapted?'documented adaptation ':'original source ')+statuses.length+' case(s) verified');
}
const report={date:new Date().toISOString(),compiler:execFileSync('g++',['--version'],{encoding:'utf8'}).split('\n')[0],standard:'c++20',workedPrograms:worked.length,sourceCppListingsCovered:covered.size,completeProgramListings:worked.filter(x=>x.sourceKind==='program').length,excerptWrappers:worked.filter(x=>x.sourceKind==='excerpt').length,checks:results.reduce((n,r)=>n+r.cases.length,0),adapted:results.filter(r=>r.adapted).length,originalUnchanged:results.filter(r=>!r.adapted).length,results};await fs.writeFile(path.join(root,'tests/book-one-worked-results.json'),JSON.stringify(report,null,2)+'\n');

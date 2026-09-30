import {sourceData,sourceCoding} from './read-source.mjs';
import fs from 'node:fs/promises';import os from 'node:os';import path from 'node:path';import {fileURLToPath} from 'node:url';import {spawnSync,execFileSync} from 'node:child_process';import assert from 'node:assert/strict';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const catalog=sourceCoding();
const programs=catalog.workedPrograms.filter(x=>x.courseId==='cpp-book-02');assert.equal(programs.length,78);
const book=sourceData().courses.find(c=>c.id==='cpp-book-02');
assert.deepEqual(new Set(programs.flatMap(w=>[w.sourceId,...w.sourcePartIds])),new Set(book.series.listings.filter(e=>e.language==='cpp').map(e=>e.id)));
const temp=await fs.mkdtemp(path.join(os.tmpdir(),'book-two-verified-')),results=[];
try{
 for(const w of programs){
  const base=path.join(root,'coding_lab/book_02_worked',w.sourceId),file=path.join(base,'main.cpp'),original=path.join(base,'original.cpp'),bin=path.join(temp,w.sourceId);
  assert.equal(await fs.readFile(file,'utf8'),w.source.endsWith('\n')?w.source:w.source+'\n');
  assert.equal(await fs.readFile(original,'utf8'),w.originalSource.endsWith('\n')?w.originalSource:w.originalSource+'\n');
  const compiled=spawnSync('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',file,'-o',bin],{encoding:'utf8',timeout:40000});assert.equal(compiled.status,0,w.sourceId+' compile: '+compiled.stderr);
  const cases=[];
  for(const c of w.checks){
   const cwd=await fs.mkdtemp(path.join(temp,'case-'));
   try{
    const result=spawnSync(bin,[],{cwd,input:c.input,encoding:'utf8',timeout:8000});assert.ok(!result.error,w.sourceId+': '+result.error);
    assert.equal(result.status,c.exitCode,w.sourceId+' '+c.label+' exit');assert.equal(result.stdout,c.stdout,w.sourceId+' '+c.label+' stdout');assert.equal(result.stderr,c.stderr,w.sourceId+' '+c.label+' stderr');
    cases.push(c.label);
   }finally{await fs.rm(cwd,{recursive:true,force:true});}
  }
  results.push({id:w.sourceId,chapter:w.chapter,adapted:w.adapted,cases});console.log(w.sourceId+' CH'+w.chapter+' '+cases.length+' case(s) verified');
 }
}finally{await fs.rm(temp,{recursive:true,force:true});}
const report={date:new Date().toISOString(),compiler:execFileSync('g++',['--version'],{encoding:'utf8'}).split('\n')[0],standard:'c++20',workshops:programs.length,cppListingsCovered:84,checks:results.reduce((n,x)=>n+x.cases.length,0),adapted:results.filter(x=>x.adapted).length,originalUnchanged:results.filter(x=>!x.adapted).length,results};
assert.equal(report.checks,81);await fs.writeFile(path.join(root,'tests/book-two-worked-results.json'),JSON.stringify(report,null,2)+'\n');

// Verify the actual complete-project ZIP, not merely the current work directory.
import fs from 'node:fs/promises';import os from 'node:os';import path from 'node:path';
import {execFile} from 'node:child_process';import {promisify} from 'node:util';import assert from 'node:assert/strict';
const exec=promisify(execFile),archive=path.resolve(process.argv[2]||'../output/Design_Patterns_Study_Studio_Source.zip');
const temporary=await fs.mkdtemp(path.join(os.tmpdir(),'studio-book-three-rebuild-'));
const report={archive:path.basename(archive),steps:[],networkUsed:false};
try{
 await exec('unzip',['-q',archive,'-d',temporary]);const cwd=path.join(temporary,'Design_Patterns_Study_Studio');
 const expected=JSON.parse(await fs.readFile(path.join(cwd,'dist/integrity.json'),'utf8'));
 // Only this extracted, disposable generated directory is removed, never the original project.
 await fs.rm(path.join(cwd,'dist'),{recursive:true,force:true});
 for(const args of [['ci','--offline'],['run','build'],['test']]){
  const r=await exec('npm',args,{cwd,timeout:120000,maxBuffer:5000000});console.log(r.stdout);report.steps.push({command:'npm '+args.join(' '),passed:true});
 }
 await fs.access(path.join(cwd,'node_modules/playwright/package.json'));
 const actual=JSON.parse(await fs.readFile(path.join(cwd,'dist/integrity.json'),'utf8'));
 assert.deepEqual(actual,expected,'Rebuilt dist differs from the distributed dist');
 report.generatedFiles=actual.files.length;report.exactDistIdentity=true;
 console.log('PASS clean ZIP extraction, npm ci --offline, build, tests, and exact generated dist identity.');
 await fs.writeFile(path.resolve(import.meta.dirname,'../tests/book-three-rebuild-results.json'),JSON.stringify(report,null,2)+'\n');
}finally{await fs.rm(temporary,{recursive:true,force:true});}

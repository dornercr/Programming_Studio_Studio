// Verify every complete lecture program with a real local C++ compiler.
// Deliberate build failures must fail to compile; runnable cases must match all output.
import fs from 'node:fs/promises';
import path from 'node:path';
import os from 'node:os';
import crypto from 'node:crypto';
import {execFile} from 'node:child_process';
import {promisify} from 'node:util';
import assert from 'node:assert/strict';
const exec=promisify(execFile),root=path.resolve(import.meta.dirname,'..');process.chdir(root);
const compiler=process.env.CXX||'g++',version=(await exec(compiler,['--version'])).stdout.split('\n')[0];
const entries=JSON.parse(await fs.readFile('lectures/manifest.json','utf8')).filter(e=>e.courseId==='design-patterns-cpp');
const jobs=[];
for(const e of entries){const d=JSON.parse(await fs.readFile(e.path,'utf8'));for(const s of d.slides){if(!s.code?.runAllowed)continue;jobs.push({id:s.id,...s.code,standard:'c++20'});for(const v of s.code.variants||[])jobs.push({...s.code,...v,id:s.id+'/'+v.id,standard:v.standard||'c++20'});}}
const dir=await fs.mkdtemp(path.join(os.tmpdir(),'dp-slides-')),results=[],errors=[],cache=new Map();let next=0;
async function check(j){
 const digest=crypto.createHash('sha256').update(j.standard+'\n'+j.text).digest('hex'),file=path.join(dir,digest+'.cpp'),bin=path.join(dir,digest);
 if(!cache.has(digest))cache.set(digest,(async()=>{await fs.writeFile(file,j.text);try{const build=await exec(compiler,['-std='+j.standard,'-Wall','-Wextra','-Wpedantic','-pthread',file,'-o',bin],{timeout:60000,maxBuffer:2*1024*1024});try{const run=await exec(bin,[],{timeout:5000,maxBuffer:2*1024*1024});return {phase:'run',stdout:run.stdout,stderr:run.stderr,exit:0,warnings:build.stderr};}catch(e){return {phase:'run',stdout:e.stdout||'',stderr:e.stderr||'',exit:e.code,signal:e.signal};}}catch(e){return {phase:'build',diagnostic:e.stderr||String(e)};}})());
 const actual=await cache.get(digest);
 if(j.expectedPhase==='build'){assert.equal(actual.phase,'build',j.id+' must not compile');}
 else {assert.equal(actual.phase,'run',j.id+' did not compile: '+actual.diagnostic);assert.equal(actual.exit,j.expectedExitCode??0,j.id+' exit');assert.equal(actual.stdout,j.expectedStdout||'',j.id+' stdout');assert.equal(actual.stderr,j.expectedStderr||'',j.id+' stderr');}
 return {id:j.id,sha256:digest,phase:actual.phase,passed:true};
}
try{
 await Promise.all(Array.from({length:Number(process.env.CPP_JOBS)||4},async()=>{while(next<jobs.length){const j=jobs[next++];try{results.push(await check(j));}catch(e){errors.push({id:j.id,error:String(e)});}if((results.length+errors.length)%30===0)console.log(`${results.length+errors.length}/${jobs.length} C++ cases checked`);}}));
 const report={compiler:version,standard:'C++20',cases:jobs.length,uniquePrograms:cache.size,passed:results.length,errors,results:results.sort((a,b)=>a.id.localeCompare(b.id))};
 await fs.writeFile('tests/design-patterns-slides-code-results.json',JSON.stringify(report,null,2)+'\n');
 assert.equal(errors.length,0,JSON.stringify(errors,null,2));console.log(`All ${jobs.length} lecture C++ cases verified (${cache.size} distinct compilations).`);
}finally{await fs.rm(dir,{recursive:true,force:true});}

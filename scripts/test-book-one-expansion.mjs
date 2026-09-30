// Compile and execute every new starter/reference with the same harness as the browser.
import fs from 'node:fs/promises';import path from 'node:path';import os from 'node:os';import assert from 'node:assert/strict';import {execFile} from 'node:child_process';import {promisify} from 'node:util';
import {bookOneExpansion as pack} from './book-one-expansion.mjs';import {labBuildProgram,labParseResponse} from '../src/coding-core.mjs';
const exec=promisify(execFile),root=path.resolve(import.meta.dirname,'..'),temp=await fs.mkdtemp(path.join(os.tmpdir(),'book-one-expansion-'));process.chdir(root);
const results=[],lines=text=>text.trimEnd().split('\n').map(text=>({text}));
try{
 for(const q of pack.questions){const row={id:q.id,checks:q.tests.length};
  for(const [kind,source,mode]of [['solution',q.solution,'test'],['sample',q.solution,'run'],['starter',q.starter,'test']]){
   const file=path.join(temp,q.id+'-'+kind+'.cpp'),binary=file.slice(0,-4);await fs.writeFile(file,labBuildProgram(source,q,mode,'local'));
   try{await exec('g++',['-std=c++20','-Wall','-Wextra','-Wpedantic',file,'-o',binary],{timeout:30000});}catch(e){throw Error(q.id+' '+kind+' compile failed: '+e.stderr);}
   let run;if(mode==='run')run=await inputRun(binary,q.sampleInput);
   else try{run=await exec(binary,[],{timeout:5000});}catch(e){if(e.signal)throw e;run={stdout:e.stdout,stderr:e.stderr,code:e.code};}
   if(kind==='sample'){assert.equal(run.code??0,0,q.id+' sample exit');assert.equal(run.stdout,q.sampleOutput,q.id+' expected sample');row.sampleVerified=true;}
   else{const result=labParseResponse({code:run.code??0,didExecute:true,stdout:lines(run.stdout),stderr:lines(run.stderr),buildResult:{code:0,stderr:[]}},q,'local');
    if(kind==='solution'){assert.ok(result.passed,q.id+' reference '+JSON.stringify(result.cases.filter(c=>!c.passed)));row.referencePassed=true;}
    else{assert.ok(!result.passed,q.id+' starter must fail');row.starterFailures=result.cases.filter(c=>!c.passed).map(c=>({label:c.label,actual:c.actual,expected:c.expected}));}
   }
  }
  results.push(row);console.log(q.id+': reference, sample, and failing starter verified');
 }
 const binary=path.join(temp,'billing');await exec('g++',['-std=c++20','-Wall','-Wextra','-Wpedantic','coding_lab/book_one_expansion/ch19/main.cpp','coding_lab/book_one_expansion/ch19/billing.cpp','-o',binary]);assert.equal((await inputRun(binary,'7 4\n')).stdout,'28\n');
 await fs.writeFile('tests/book-one-expansion-verification.json',JSON.stringify({compiler:(await exec('g++',['--version'])).stdout.split('\n')[0],questions:results.length,checks:results.reduce((n,r)=>n+r.checks,0),multiFileVerified:true,results},null,2)+'\n');
}finally{await fs.rm(temp,{recursive:true,force:true});}
async function inputRun(binary,input){const {spawn}=await import('node:child_process');return new Promise((resolve,reject)=>{const child=spawn(binary,[],{stdio:['pipe','pipe','pipe']});let stdout='',stderr='';const timer=setTimeout(()=>{child.kill();reject(Error('Execution timeout'));},5000);child.stdout.on('data',x=>stdout+=x);child.stderr.on('data',x=>stderr+=x);child.stdin.on('error',e=>{if(e.code!=='EPIPE')reject(e);});child.on('error',reject);child.on('close',code=>{clearTimeout(timer);resolve({stdout,stderr,code});});child.stdin.end(input);});}

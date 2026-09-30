// This verifier also runs from the extracted companion without npm or network.
import fs from 'node:fs/promises';import path from 'node:path';import os from 'node:os';
import {fileURLToPath} from 'node:url';import {execFile} from 'node:child_process';import {promisify} from 'node:util';import assert from 'node:assert/strict';
const directory=path.dirname(fileURLToPath(import.meta.url)),exec=promisify(execFile);
const standalone=await fs.access(path.join(directory,'catalog.json')).then(()=>true,()=>false);
const root=standalone?directory:path.resolve(directory,'..');
const {labBuildProgram,labParseResponse}=await import(standalone?'./coding-core.mjs':'../src/coding-core.mjs');
let catalog;if(standalone)catalog=JSON.parse(await fs.readFile(path.join(root,'catalog.json'),'utf8'));
else {const {sourceCoding}=await import('./read-source.mjs');const all=sourceCoding();catalog={questions:all.questions.filter(q=>q.courseId==='cpp-book-03'),workedPrograms:all.workedPrograms.filter(w=>w.courseId==='cpp-book-03')};}
assert.equal(catalog.questions.length,22);assert.equal(catalog.workedPrograms.length,66);
const temp=await fs.mkdtemp(path.join(os.tmpdir(),'book-three-verify-')),results=[];
async function compile(code,name){const source=path.join(temp,name+'.cpp'),binary=path.join(temp,name);await fs.writeFile(source,code);try{await exec('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',source,'-o',binary],{timeout:45000,maxBuffer:2000000});}catch(e){throw Error(name+' compile: '+e.stderr);}return binary;}
// execFile does not take an input option; write it to the child's stdin explicitly.
const {spawn}=await import('node:child_process');
async function execute(binary,input='',cwd=temp){return new Promise((resolve,reject)=>{const child=spawn(binary,[],{cwd,stdio:['pipe','pipe','pipe']});let stdout='',stderr='',done=false;const timer=setTimeout(()=>{child.kill();reject(Error('Execution timeout: '+binary));},10000);child.stdout.on('data',b=>stdout+=b);child.stderr.on('data',b=>stderr+=b);child.on('error',e=>{clearTimeout(timer);reject(e);});child.stdin.on('error',e=>{if(e.code!=='EPIPE')reject(e);});child.on('close',(status,signal)=>{clearTimeout(timer);done=true;if(signal)reject(Error(signal));else resolve({stdout,stderr,status});});child.stdin.end(input);});}
const lines=s=>s.replace(/\n+$/,'').split('\n').map(text=>({text}));
const parse=(r,q)=>labParseResponse({code:r.status,didExecute:true,stdout:lines(r.stdout),stderr:lines(r.stderr),buildResult:{code:0,stderr:[]}},q,'offline');
const badSamples={
 'b3-first-maximum':'2\n','b3-pair-count':'16\n','b3-erase-index':'true\n7 8 \n',
 'b3-reverse-chain':'1 2 3 \n','b3-ring-queue':'7 7 7\n','b3-ordered-frequency':'-2:1\n1:1\n3:1\n',
 'b3-tree-height':'1\n','b3-bst-insert':'5 \n','b3-right-rotation':'3 3\n','b3-max-heap':'2 4 9 \n',
 'b3-adjacency':'0:\n1:0 2 \n2:0 \n','b3-bfs-distance':'0 -1 -1 -1 \n',
 'b3-stable-sort':'1:B 2:C 2:A \n','b3-binary-choices':'[00]\n','b3-intervals':'1\n',
 'b3-minimum-coins':'-1\n','b3-matrix-layout':'6\n','b3-path-witness':'true\n','b3-transactional-routes':'1 0 0 0\n'
};
async function question(q){
 const folder=path.join(root,standalone?'questions':'coding_lab',q.id);
 for(const [name,key]of [['starter.cpp','starter'],['solution.cpp','solution'],['driver.cpp','driver']])assert.equal(await fs.readFile(path.join(folder,name),'utf8'),q[key]);
 const sample=await execute(await compile(labBuildProgram(q.solution,q,'run'),q.id+'-sample'),q.sampleInput);assert.equal(sample.status,0);assert.equal(sample.stdout,q.sampleOutput,q.id+' sample');assert.equal(sample.stderr,'');
 const good=await execute(await compile(labBuildProgram(q.solution,q,'test','offline'),q.id+'-tests'));assert.ok(parse(good,q).passed,q.id+' reference: '+good.stdout);
 const starter=await execute(await compile(labBuildProgram(q.starter,q,'test','offline'),q.id+'-starter-tests'));const broken=parse(starter,q);assert.ok(!broken.passed,q.id+' starter must expose a failure');
 const bad=await execute(await compile(labBuildProgram(q.starter,q,'run'),q.id+'-starter-sample'),q.sampleInput);assert.equal(bad.status,0);assert.equal(bad.stderr,'');if(q.id in badSamples)assert.equal(bad.stdout,badSamples[q.id],q.id+' printed incorrect-output explanation');
 return {kind:'question',id:q.id,chapter:q.chapter,checks:q.tests.length,sampleVerified:true,starterFailed:broken.cases.filter(c=>!c.passed).map(c=>c.label),starterSampleOutput:bad.stdout};
}
async function workshop(w){
 const folder=path.join(root,standalone?'worked':'coding_lab/book_03_worked',w.sourceId);
 const code=await fs.readFile(path.join(folder,'main.cpp'),'utf8'),original=await fs.readFile(path.join(folder,'original.cpp'),'utf8');assert.equal(code.trimEnd(),w.source.trimEnd());assert.equal(original.trimEnd(),w.originalSource.trimEnd());
 const binary=await compile(code,w.sourceId);
 for(const check of w.checks){const cwd=await fs.mkdtemp(path.join(temp,'case-'));try{const r=await execute(binary,check.input,cwd);assert.equal(r.status,check.exitCode,w.sourceId+' exit');assert.equal(r.stdout,check.stdout,w.sourceId+' stdout');assert.equal(r.stderr,check.stderr,w.sourceId+' stderr');}finally{await fs.rm(cwd,{recursive:true,force:true});}}
 return {kind:'workshop',id:w.sourceId,chapter:w.chapter,checks:w.checks.length,adapted:w.adapted,sourceParts:w.sourcePartIds,assertionChecks:Number(w.sampleOutput.match(/PASS checks=(\d+)/)?.[1]||0)};
}
const jobs=[...catalog.questions.map(q=>()=>question(q)),...catalog.workedPrograms.map(w=>()=>workshop(w))];let index=0;
try{let failure;await Promise.all(Array.from({length:4},async()=>{while(index<jobs.length&&!failure){const n=index++;try{const r=await jobs[n]();results[n]=r;console.log('PASS '+r.id+' '+r.checks+' check(s)');}catch(e){failure=e;}}}));if(failure)throw failure;}
catch(e){await fs.rm(temp,{recursive:true,force:true});throw e;}
try{if(standalone){
 const project=path.join(root,'projects/harbor-routes');
 for(const [main,name]of [['main.cpp','route-app'],['tests.cpp','route-tests']]){
  const binary=path.join(temp,name);await exec('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',path.join(project,'routes.cpp'),path.join(project,main),'-o',binary],{timeout:45000});
  if(main==='main.cpp')for(const c of catalog.workedPrograms.find(w=>w.sourceId==='B03-L0093').checks){const r=await execute(binary,c.input);assert.equal(r.status,c.exitCode);assert.equal(r.stdout,c.stdout);assert.equal(r.stderr,c.stderr);}
  else {const r=await execute(binary);assert.equal(r.status,0);assert.equal(r.stdout,catalog.workedPrograms.find(w=>w.sourceId==='B03-L0094').sampleOutput);assert.equal(r.stderr,'');}
 }
 console.log('PASS original multi-file HarborRoutes application and independent invariant/witness tests.');
}}finally{await fs.rm(temp,{recursive:true,force:true});}
const report={compiler:(await exec('g++',['--version'])).stdout.split('\n')[0],standard:'c++20',networkUsed:false,questions:22,questionChecks:results.filter(r=>r.kind==='question').reduce((n,r)=>n+r.checks,0),workshops:66,workshopChecks:results.filter(r=>r.kind==='workshop').reduce((n,r)=>n+r.checks,0),cppListings:68,internalAssertions:results.reduce((n,r)=>n+(r.assertionChecks||0),0),results};
assert.equal(report.questionChecks,116);assert.equal(report.workshopChecks,71);
if(!standalone)await fs.writeFile(path.join(root,'tests/book-three-verification.json'),JSON.stringify(report,null,2)+'\n');
console.log(`PASS: 22 reference solutions and deliberately failing starters; 116 question checks; 66 workshops; 71 behavior cases; ${report.internalAssertions} internal assertions. No network used.`);

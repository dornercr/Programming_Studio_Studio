// Run inside the extracted Book II companion. Requires Node.js and local g++ only.
import fs from 'node:fs/promises';import path from 'node:path';import os from 'node:os';import {fileURLToPath} from 'node:url';import {spawnSync} from 'node:child_process';import assert from 'node:assert/strict';
import {labBuildProgram,labParseResponse} from './coding-core.mjs';
const root=path.dirname(fileURLToPath(import.meta.url)),catalog=JSON.parse(await fs.readFile(path.join(root,'catalog.json'),'utf8'));
const temp=await fs.mkdtemp(path.join(os.tmpdir(),'book-two-offline-'));
const lines=s=>String(s||'').replace(/\n+$/,'').split('\n').map(text=>({text}));
let qChecks=0,wChecks=0;
async function compile(source,name){const filename=path.join(temp,name+'.cpp'),binary=path.join(temp,name);await fs.writeFile(filename,source);const r=spawnSync('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',filename,'-o',binary],{encoding:'utf8',timeout:45000});assert.equal(r.status,0,name+': '+r.stderr);return binary;}
function run(binary,input='',cwd=temp){const r=spawnSync(binary,[],{input,cwd,encoding:'utf8',timeout:8000});assert.ok(!r.error,String(r.error));return r;}
try{
 for(const q of catalog.questions){
  const folder=path.join(root,'questions',q.id);
  for(const [name,field] of [['starter.cpp','starter'],['solution.cpp','solution'],['driver.cpp','driver']])assert.equal(await fs.readFile(path.join(folder,name),'utf8'),q[field]);
  const sample=run(await compile(labBuildProgram(q.solution,q,'run'),q.id+'-sample'),q.sampleInput);assert.equal(sample.status,0,q.id+' sample exit');assert.equal(sample.stdout,q.sampleOutput,q.id+' sample output');
  const good=run(await compile(labBuildProgram(q.solution,q,'test','offline'),q.id+'-tests'));
  const parse=r=>labParseResponse({code:r.status,didExecute:true,stdout:lines(r.stdout),stderr:lines(r.stderr),buildResult:{code:0,stderr:[]}},q,'offline');
  assert.ok(parse(good).passed,q.id+' reference checks');
  const starter=run(await compile(labBuildProgram(q.starter,q,'test','offline'),q.id+'-starter'));
  assert.ok(!parse(starter).passed,q.id+' starter must show at least one failure');qChecks+=q.tests.length;
 }
 for(const w of catalog.workedPrograms){
  const folder=path.join(root,'worked',w.sourceId),code=await fs.readFile(path.join(folder,'main.cpp'),'utf8'),original=await fs.readFile(path.join(folder,'original.cpp'),'utf8');
  assert.equal(code.trimEnd(),w.source.trimEnd());assert.equal(original.trimEnd(),w.originalSource.trimEnd());
  const binary=await compile(code,w.sourceId);
  for(const check of w.checks){const cwd=await fs.mkdtemp(path.join(temp,'case-'));try{const r=run(binary,check.input,cwd);assert.equal(r.status,check.exitCode,w.sourceId+' exit');assert.equal(r.stdout,check.stdout,w.sourceId+' stdout');assert.equal(r.stderr,check.stderr,w.sourceId+' stderr');wChecks++;}finally{await fs.rm(cwd,{recursive:true,force:true});}}
 }
}finally{await fs.rm(temp,{recursive:true,force:true});}
assert.equal(catalog.questions.length,24);assert.equal(qChecks,104);assert.equal(catalog.workedPrograms.length,78);assert.equal(wChecks,81);
console.log(`PASS · ${catalog.questions.length} reference solutions and deliberately failing starters, ${qChecks} question checks, ${catalog.workedPrograms.length} workshops, ${wChecks} output/exit cases. No network used.`);

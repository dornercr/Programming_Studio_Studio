// Run inside the Book I-only companion directory. No network requests.
import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath} from 'node:url';import {spawnSync} from 'node:child_process';import assert from 'node:assert/strict';
import {labBuildProgram,labParseResponse} from './coding-core.mjs';
const root=path.dirname(fileURLToPath(import.meta.url)),catalog=JSON.parse(await fs.readFile(path.join(root,'catalog.json'),'utf8'));
const build=path.join(root,'build');await fs.mkdir(build,{recursive:true});
const lines=s=>String(s||'').replace(/\n+$/,'').split('\n').map(text=>({text}));
const compile=(source,name)=>{const file=path.join(build,name+'.cpp'),bin=path.join(build,name);return fs.writeFile(file,source).then(()=>{const r=spawnSync('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',file,'-o',bin],{encoding:'utf8',timeout:30000});assert.equal(r.status,0,name+' compiler error: '+r.stderr);return bin;});};
const run=(bin,input='')=>{const r=spawnSync(bin,[],{input,encoding:'utf8',timeout:5000});assert.ok(!r.error,'Execution interrupted: '+r.error);return r;};
let questionCases=0,workedCases=0;
for(const q of catalog.questions){
 const base=path.join(root,'questions',q.id);
 assert.equal(await fs.readFile(path.join(base,'starter.cpp'),'utf8'),q.starter);
 assert.equal(await fs.readFile(path.join(base,'solution.cpp'),'utf8'),q.solution);
 assert.equal(await fs.readFile(path.join(base,'driver.cpp'),'utf8'),q.driver);
 const sample=run(await compile(labBuildProgram(q.solution,q,'run'),q.id+'-sample'),q.sampleInput);assert.equal(sample.status,0,q.id+' sample exit');assert.equal(sample.stdout,q.sampleOutput,q.id+' sample output');
 const good=run(await compile(labBuildProgram(q.solution,q,'test','local'),q.id+'-tests'));
 const checked=labParseResponse({code:good.status,didExecute:true,stdout:lines(good.stdout),stderr:lines(good.stderr),buildResult:{code:0,stderr:[]}},q,'local');assert.ok(checked.passed,q.id+' failed: '+JSON.stringify(checked.cases.filter(c=>!c.passed)));
 const bad=run(await compile(labBuildProgram(q.starter,q,'test','local'),q.id+'-starter'));
 const starter=labParseResponse({code:bad.status,didExecute:true,stdout:lines(bad.stdout),stderr:lines(bad.stderr),buildResult:{code:0,stderr:[]}},q,'local');assert.ok(!starter.passed,q.id+' starter should reveal a failure');questionCases+=q.tests.length;
}
for(const w of catalog.workedPrograms){
 const base=path.join(root,'worked',w.sourceId),code=await fs.readFile(path.join(base,'main.cpp'),'utf8'),original=await fs.readFile(path.join(base,'original.cpp'),'utf8');
 assert.equal(code.trimEnd(),w.source.trimEnd());assert.equal(original.trimEnd(),w.originalSource.trimEnd());
 const bin=await compile(code,w.sourceId);for(const check of w.checks){const r=run(bin,check.input);assert.equal(r.status,check.exitCode,w.sourceId+' exit');assert.equal(r.stdout,check.stdout,w.sourceId+' stdout');assert.equal(r.stderr,check.stderr,w.sourceId+' stderr');workedCases++;}
}
assert.equal(catalog.questions.length,23);assert.equal(catalog.workedPrograms.length,86);assert.equal(questionCases,108);assert.equal(workedCases,90);
console.log(`PASS · ${catalog.questions.length} reference solutions, their sample outputs and intentionally failing starters; ${questionCases} question checks; ${catalog.workedPrograms.length} book workshops; ${workedCases} exact-output/exit checks. No network used.`);

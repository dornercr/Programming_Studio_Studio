import {sourceData,sourceCoding} from './read-source.mjs';
import fs from 'node:fs/promises';import path from 'node:path';import {fileURLToPath} from 'node:url';import {execFileSync} from 'node:child_process';import assert from 'node:assert/strict';import {labBuildProgram,labParseResponse} from '../src/coding-core.mjs';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const questions=sourceCoding().questions;
const build=path.join(root,'coding_lab/build');await fs.mkdir(build,{recursive:true});
const results=[];
const lines=text=>text.trimEnd().split('\n').map(text=>({text}));
for(const q of questions){
 const row={id:q.id,tests:q.tests.length};
 for(const [variant,code,mode] of [['solution-tests',q.solution,'test'],['solution-sample',q.solution,'run'],['starter-tests',q.starter,'test']]){
  const src=path.join(build,q.id+'-'+variant+'.cpp'),exe=src.slice(0,-4);await fs.writeFile(src,labBuildProgram(code,q,mode,'local'));
  try{execFileSync('g++',['-std=c++20','-Wall','-Wextra','-pedantic','-pthread',src,'-o',exe],{timeout:30000,encoding:'utf8',stdio:'pipe'});}catch(e){throw new Error(q.id+' '+variant+' compile: '+e.stderr);}
  let stdout='',stderr='',status=0;try{stdout=execFileSync(exe,[],{input:mode==='run'?q.sampleInput:'',encoding:'utf8',timeout:5000,stdio:'pipe'});}catch(e){stdout=e.stdout||'';stderr=e.stderr||'';status=e.status;if(e.signal)throw new Error(q.id+' terminated: '+e.signal);}
  if(mode==='run'){assert.equal(status,0,q.id+' sample exit');assert.equal(stdout,q.sampleOutput,q.id+' sample output');row.sampleOutputVerified=true;}
  else{const parsed=labParseResponse({code:status,didExecute:true,stdout:lines(stdout),stderr:lines(stderr),buildResult:{code:0,stderr:[]}},q,'local');if(variant==='solution-tests'){assert.ok(parsed.passed,q.id+' solution: '+JSON.stringify(parsed.cases.filter(x=>!x.passed)));row.allChecksPassed=true;}else{assert.ok(!parsed.passed,q.id+' starter must expose a failure');row.starterFailedChecks=parsed.cases.filter(x=>!x.passed).map(x=>x.label);}}
 }
 results.push(row);console.log(q.id+': reference checks + sample verified; starter exposes '+row.starterFailedChecks.length+' failures');
}
const report={date:new Date().toISOString(),compiler:execFileSync('g++',['--version'],{encoding:'utf8'}).split('\n')[0],standard:'c++20',questions:questions.length,checks:questions.reduce((n,q)=>n+q.tests.length,0),results};await fs.writeFile(path.join(root,'tests/coding-question-results.json'),JSON.stringify(report,null,2)+'\n');

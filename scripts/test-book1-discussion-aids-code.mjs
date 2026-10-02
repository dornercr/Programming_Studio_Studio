import fs from 'node:fs/promises';
import path from 'node:path';
import assert from 'node:assert/strict';
import {spawnSync} from 'node:child_process';
import {openSync,closeSync} from 'node:fs';
const root=path.resolve(import.meta.dirname,'..');process.chdir(root);
const dir='build/book1-discussion-aids-code';await fs.mkdir(dir,{recursive:true});
const jobs=new Map();
for(let n=0;n<27;n++){
  const d=JSON.parse(await fs.readFile(`lectures/cpp-book-01/ch${n}.json`,'utf8'));
  for(const e of Object.values(d.bookOneDiscussionAids.examples))if(e.fullSlideId){
    const s=d.slides.find(s=>s.id===e.fullSlideId);
    jobs.set(e.filename,{e,code:s.code});
  }
}
let index=0,strictSamples=0;
for(const [file,{e,code}] of jobs){
  if(process.argv.includes('--projects-only')&&!e.adaptationNote)continue;
  const exe=path.resolve(dir,'program-'+index++);
  const build=spawnSync('g++',['-std=c++20','-Wall','-Wextra','-Wpedantic','-pthread',e.adaptationNote?code.downloadPath:file,'-o',exe],{encoding:'utf8',timeout:60000});
  assert.equal(build.status,0,`${file}: ${build.stderr}`);
  const inputFile=path.join(dir,'input.txt');await fs.writeFile(inputFile,code.input||'');
  const fd=openSync(inputFile,'r');
  let run;try{run=spawnSync(exe,[],{stdio:[fd,'pipe','pipe'],encoding:'utf8',timeout:10000});}finally{closeSync(fd);}
  assert.equal(run.status,code.expectedExitCode,`${file}: ${run.stderr}`);
  if(!e.recordedObservation||e.adaptationNote){assert.equal(run.stdout,e.output,file);assert.equal(run.stderr,code.expectedStderr);strictSamples++;}
  console.log(`Verified ${file}${e.recordedObservation?' (recorded output may vary)':' and exact sample output'}`);
}
console.log(`${index} source programs compiled and ran; ${strictSamples} exact supplied samples matched.`);

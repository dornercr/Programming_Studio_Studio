import fs from 'node:fs/promises';
import path from 'node:path';
import os from 'node:os';
import {spawnSync} from 'node:child_process';
import assert from 'node:assert/strict';

const root = path.resolve(import.meta.dirname,'..');
const dir = await fs.mkdtemp(path.join(os.tmpdir(),'systems-aids-cpp-'));
const records = [];
try {
  for(let chapter=1;chapter<=61;chapter++) {
    const chunk = JSON.parse(await fs.readFile(path.join(root,`content/systems-programming/ch${chapter}.json`),'utf8'));
    const source = chunk.examples[chapter].examples;
    const binary = path.join(dir,`ch${chapter}`);
    const build = spawnSync(process.env.CXX || 'g++', ['-std=c++20','-Wall','-Wextra','-Wpedantic','-Werror','-pthread',path.join(root,source.filename),'-o',binary], {encoding:'utf8',timeout:30000});
    assert.equal(build.status,0,`Chapter ${chapter} build: ${build.stderr}`);
    const result = spawnSync(binary,[],{encoding:'utf8',timeout:5000});
    assert.equal(result.status,0,`Chapter ${chapter} run: ${result.stderr}`);
    assert.equal(result.stdout,source.output,`Chapter ${chapter} stdout`);
    assert.equal(result.stderr,'',`Chapter ${chapter} stderr`);
    records.push({chapter,source:source.filename,compiled:true,outputMatch:true});
    console.log(`Chapter ${chapter}: compiled and exact output matched.`);
  }
  await fs.mkdir(path.join(root,'build/systems-discussion-aids-qa'),{recursive:true});
  await fs.writeFile(path.join(root,'build/systems-discussion-aids-qa/cpp-results.json'),JSON.stringify({programs:records},null,2)+'\n');
} finally { await fs.rm(dir,{recursive:true,force:true}); }

import fs from 'node:fs/promises';
import path from 'node:path';
import {spawnSync} from 'node:child_process';
import assert from 'node:assert/strict';

const root = path.resolve(import.meta.dirname, '..');
process.chdir(root);
const dir = 'build/design-patterns-discussion-aids-code';
await fs.mkdir(dir, {recursive:true});
for (let chapter=0; chapter<24; chapter++) {
  const deck = JSON.parse(await fs.readFile(`lectures/design-patterns-cpp/ch${chapter}.json`,'utf8'));
  const example = deck.designPatternsDiscussionAids.examples[0];
  const program = deck.slides.find(s => s.id === example.fullSlideId).code;
  const output = path.resolve(dir, 'ch' + chapter);
  const compile = spawnSync('g++', ['-std=c++20','-Wall','-Wextra','-Wpedantic','-pthread',program.downloadPath,'-o',output], {encoding:'utf8',timeout:60000});
  assert.equal(compile.status,0,compile.stderr);
  const run = spawnSync(output,[],{encoding:'utf8',timeout:10000});
  assert.equal(run.status,program.expectedExitCode);
  assert.equal(run.stdout,program.expectedStdout);
  assert.equal(run.stderr,program.expectedStderr);
  console.log(`Chapter ${chapter}: C++20 compiled and exact output matched.`);
}

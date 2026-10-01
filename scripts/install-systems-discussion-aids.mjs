import fs from 'node:fs/promises';
import path from 'node:path';
import {spawnSync} from 'node:child_process';

const root = path.resolve(import.meta.dirname, '..');
const read = file => fs.readFile(path.join(root, file), 'utf8');
const hook = "await import('./build-systems-discussion-aids.mjs');";
const jsBegin = '  // BEGIN SYSTEMS DISCUSSION AIDS';
const jsEnd = '  // END SYSTEMS DISCUSSION AIDS';
const cssBegin = '/* BEGIN SYSTEMS DISCUSSION AIDS */';
const cssEnd = '/* END SYSTEMS DISCUSSION AIDS */';

function replaceSection(text, start, end, addition) {
  const first = text.indexOf(start), last = text.indexOf(end);
  if ((first < 0) !== (last < 0) || (first >= 0 && last < first))
    throw new Error('An incomplete discussion-aids section was found. No changes were made.');
  if (first < 0) return text.trimEnd() + '\n\n' + addition.trimEnd() + '\n';
  return text.slice(0, first) + addition.trimEnd() + text.slice(last + end.length);
}

const original = Object.fromEntries(await Promise.all(['src/slides.js', 'src/slides.css', 'scripts/build-slides.mjs'].map(async file => [file, await read(file)])));
let renderer = original['src/slides.js'];
if (!renderer.includes('function slidesWorkbench') || !renderer.includes('function slidesExcerpt') || !renderer.includes('function slidesGo'))
  throw new Error('This project does not have the expected working slide renderer. No changes were made.');
renderer = replaceSection(renderer, jsBegin, jsEnd, await read('src/systems-discussion-aids.js'));
if (!renderer.includes('if(s.discussionAid)return systemsDiscussionWorkbench(s);')) {
  const marker = /function slidesWorkbench\(s\)\s*\{/;
  if (!marker.test(renderer)) throw new Error('Cannot locate the slide workbench. No changes were made.');
  renderer = renderer.replace(marker, '$&\n    if(s.discussionAid)return systemsDiscussionWorkbench(s);');
}
const css = replaceSection(original['src/slides.css'], cssBegin, cssEnd, await read('src/systems-discussion-aids.css'));
let builder = original['scripts/build-slides.mjs'];
if (!builder.includes(hook)) {
  const marker = /^\s*const entries\s*=\s*JSON\.parse/m;
  if (!marker.test(builder)) throw new Error('Cannot locate the lecture catalog build step. No changes were made.');
  builder = builder.replace(marker, '\n' + hook + '\nconst entries=JSON.parse');
}
// The generated integrity manifest cannot carry a valid hash of itself.
const integrityWrite = "await fs.writeFile('dist/integrity.json',JSON.stringify(integrity));";
const integrityGuard = "integrity.files=integrity.files.filter(x=>x.file!=='integrity.json');";
if (builder.includes(integrityWrite) && !builder.includes(integrityGuard))
  builder = builder.replace(integrityWrite, integrityGuard + '\n ' + integrityWrite);
const changed = {'src/slides.js': renderer, 'src/slides.css': css, 'scripts/build-slides.mjs': builder};
for (const file of ['src/slides.js', 'scripts/build-slides.mjs']) {
  const result = spawnSync(process.execPath, ['--input-type=module', '--check'], {input: changed[file], encoding: 'utf8'});
  if (result.status !== 0) throw new Error(`Syntax check failed for ${file}. No changes were made.\n${result.stderr}`);
}
const entries = JSON.parse(await read('lectures/manifest.json')).filter(e => e.courseId === 'systems-programming');
if (entries.length !== 61 || new Set(entries.map(e => Number(e.chapter))).size !== 61)
  throw new Error('Install the working Systems lectures for all 61 chapters first. No changes were made.');
if (process.argv.includes('--check')) {
  console.log('Discussion-aids installer is compatible. No files changed.');
} else {
  const stamp = new Date().toISOString().replace(/[:.]/g, '-');
  let count = 0;
  for (const [file, text] of Object.entries(changed)) {
    if (text === original[file]) continue;
    const backup = file + '.before-systems-discussion-' + stamp;
    await fs.copyFile(path.join(root, file), path.join(root, backup), fs.constants.COPYFILE_EXCL);
    await fs.writeFile(path.join(root, file), text);
    console.log(`Updated ${file}; backup: ${backup}`);
    count++;
  }
  console.log(count ? 'Installed. Run npm run build, then npm test.' : 'Already installed; no files changed.');
}

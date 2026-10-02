import fs from 'node:fs/promises';
import path from 'node:path';
import {spawnSync} from 'node:child_process';

const root = path.resolve(import.meta.dirname, '..');
const read = file => fs.readFile(path.join(root, file), 'utf8');
const hook = "await import('./build-book1-discussion-aids.mjs');";
const jsBegin = '  // BEGIN BOOK I DISCUSSION AIDS';
const jsEnd = '  // END BOOK I DISCUSSION AIDS';
const cssBegin = '/* BEGIN BOOK I DISCUSSION AIDS */';
const cssEnd = '/* END BOOK I DISCUSSION AIDS */';

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
renderer = replaceSection(renderer, jsBegin, jsEnd, await read('src/book1-discussion-aids.js'));
if(!renderer.includes('if(s.bookOneProjectExample)return bookOneProjectWorkbench(s);'))
  renderer=renderer.replace(/function slidesWorkbench\(s\)\s*\{/,'$&\n    if(s.bookOneProjectExample)return bookOneProjectWorkbench(s);');
if(!renderer.includes('if(s.bookOneDiagram)return bookOneDiagramWorkbench(s);'))
  renderer=renderer.replace(/function slidesWorkbench\(s\)\s*\{/,'$&\n    if(s.bookOneDiagram)return bookOneDiagramWorkbench(s);');
if (!renderer.includes('if(s.bookOneDiscussionAid)return bookOneDiscussionWorkbench(s);')) {
  const marker = /function slidesWorkbench\(s\)\s*\{/;
  if (!marker.test(renderer)) throw new Error('Cannot locate the slide workbench. No changes were made.');
  renderer = renderer.replace(marker, '$&\n    if(s.bookOneDiscussionAid)return bookOneDiscussionWorkbench(s);');
}
const css = replaceSection(original['src/slides.css'], cssBegin, cssEnd, await read('src/book1-discussion-aids.css'));
if (!renderer.includes('room.dataset.slidesCourse=state.courseId;')) {
  const marker='room.innerHTML=renderSlides();';
  if(!renderer.includes(marker))throw Error('Cannot locate the lecture workspace. No changes were made.');
  renderer=renderer.replace(marker,'room.dataset.slidesCourse=state.courseId;'+marker);
}
if(!renderer.includes("bookOneOpenDiagram($('.slides-diagram'),el)")){
  const marker="case 'slides-diagram':{";
  if(!renderer.includes(marker))throw Error('Cannot locate the diagram control. No changes were made.');
  renderer=renderer.replace(marker,marker+"if(slidesDeck()?.courseId==='cpp-book-01'){bookOneOpenDiagram($('.slides-diagram'),el);break;}");
}
if(!renderer.includes('if(s.code.recordedObservation)return bookOneRecordedOutput(s);')){
  const marker=/function slidesExpected\(s\)\s*\{/;
  if(!marker.test(renderer))throw Error('Cannot locate expected-output rendering. No changes were made.');
  renderer=renderer.replace(marker,'$&if(s.code.recordedObservation)return bookOneRecordedOutput(s);');
}
if(!renderer.includes("d.input=s.id.startsWith('b01-discussion-program-')")){
  const marker="if(typeof d.input!=='string')d.input='';";
  if(!renderer.includes(marker))throw Error('Cannot locate input initialization. No changes were made.');
  renderer=renderer.replace(marker,"if(typeof d.input!=='string')d.input=s.id.startsWith('b01-discussion-program-')?(s.code.input||''):'';");
}
const scrollHint='<p class="slides-network">Scroll horizontally to follow the full diagram at readable text size.</p>';
if(!renderer.includes("slidesDeck()?.courseId==='cpp-book-01'?'':"))
  renderer=renderer.replaceAll(scrollHint,"${slidesDeck()?.courseId==='cpp-book-01'?'':'"+scrollHint+"'}");
// Keep lecture shortcuts behind the modal so Escape closes only the diagram.
if (!renderer.includes("document.querySelector('.b1-aid-viewer[open]')")) {
  const marker = "state.mode!=='slides'||DIALOG.open";
  if (!renderer.includes(marker)) throw new Error('Cannot locate the lecture keyboard guard. No changes were made.');
  renderer = renderer.replace(marker, marker + "||document.querySelector('.b1-aid-viewer[open]')");
}
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
const entries = JSON.parse(await read('lectures/manifest.json')).filter(e => e.courseId === 'cpp-book-01');
if (entries.length !== 27 || new Set(entries.map(e => Number(e.chapter))).size !== 27)
  throw new Error('Install the authored Book I lectures for chapters 0-26 first. No changes were made.');
if (process.argv.includes('--check')) {
  console.log('Discussion-aids installer is compatible. No files changed.');
} else {
  const stamp = new Date().toISOString().replace(/[:.]/g, '-');
  let count = 0;
  for (const [file, text] of Object.entries(changed)) {
    if (text === original[file]) continue;
    const backup = file + '.before-book1-discussion-' + stamp;
    await fs.copyFile(path.join(root, file), path.join(root, backup), fs.constants.COPYFILE_EXCL);
    await fs.writeFile(path.join(root, file), text);
    console.log(`Updated ${file}; backup: ${backup}`);
    count++;
  }
  console.log(count ? 'Installed. Run npm run build, then npm test.' : 'Already installed; no files changed.');
}

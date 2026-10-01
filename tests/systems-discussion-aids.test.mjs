import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import path from 'node:path';
import os from 'node:os';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';
import {models, chooseModel} from '../scripts/systems-discussion-models.mjs';

const root = path.resolve(import.meta.dirname, '..');
const read = async file => JSON.parse(await fs.readFile(path.join(root, file), 'utf8'));
const hash = text => crypto.createHash('sha256').update(text).digest('hex');
const entries = (await read('lectures/manifest.json')).filter(e => e.courseId === 'systems-programming');
const media = s => s.code || s.diagram || s.visual || s.stages?.length || s.walkthrough?.length ||
  s.sequence?.length || s.excerpt || s.excerpts?.length;

test('All 61 Systems chapters and every otherwise text-only slide have discussion aids', async () => {
  assert.equal(entries.length, 61);
  assert.deepEqual(entries.map(e => Number(e.chapter)).sort((a,b) => a-b), Array.from({length:61}, (_,i) => i+1));
  let total = 0, discussions = 0;
  for (const entry of entries) {
    const deck = await read(entry.path);
    assert.ok(deck.discussionAids?.example);
    for (const slide of deck.slides) {
      if (media(slide)) { assert.ok(!slide.discussionAid); continue; }
      const graph = deck.discussionAids.graphs[slide.discussionAid?.graph];
      assert.ok(graph, `${entry.chapter}: ${slide.title}`);
      assert.equal(graph.title, chooseModel(entry.chapter, slide.title).title);
      assert.match(graph.image, /^data:image\/svg\+xml;base64,/);
      const svg = Buffer.from(graph.image.split(',')[1], 'base64').toString('utf8');
      assert.match(svg, /<svg/);
      assert.doesNotMatch(svg, /<script|<foreignObject|href="https?:/);
      total++;
      if (slide.reading?.length) discussions++;
    }
  }
  const report = await read('docs/systems-discussion-aids-coverage.json');
  assert.equal(report.totals.pages, total);
  assert.equal(report.totals.discussionPages, discussions);
  assert.ok(discussions >= 1336);
});

test('Discussion C++ excerpts, output, and run links equal actual delivered chapter programs', async () => {
  for (const entry of entries) {
    const deck = await read(entry.path), source = await read(`content/systems-programming/ch${entry.chapter}.json`);
    const aid = deck.discussionAids.example, program = source.examples[entry.chapter].examples;
    const code = await fs.readFile(path.join(root, aid.filename), 'utf8');
    assert.equal(aid.filename, program.filename);
    assert.equal(aid.sourceHash, hash(code));
    assert.equal(aid.text, code.split('\n').slice(aid.lineStart-1, aid.lineEnd).join('\n'));
    assert.equal(aid.output, program.output);
    const linked = deck.slides.find(s => s.id === aid.fullSlideId);
    assert.ok(linked?.code?.runAllowed);
    assert.equal(linked.code.text, code);
    assert.equal(linked.code.expectedStdout, aid.output);
  }
});

test('Every discussion diagram has explicit, valid relationships and readable source-independent labels', () => {
  assert.equal(Object.keys(models).length, 61);
  for (const list of Object.values(models)) for (const model of list) {
    const ids = new Set(model.nodes.map(n => n.id));
    assert.equal(ids.size, model.nodes.length);
    assert.ok(model.caption && model.edges.length >= 3);
    for (const edge of model.edges) {
      assert.ok(ids.has(edge.from) && ids.has(edge.to));
      assert.ok(edge.label);
    }
    for (const node of model.nodes) if (node.match) assert.doesNotThrow(() => new RegExp(node.match, 'i'));
    if (model.match) assert.doesNotThrow(() => new RegExp(model.match, 'i'));
  }
});

test('Adding discussion aids keeps every retained original lesson object unchanged', async () => {
  for (const entry of entries) {
    const deck = await read(entry.path), source = await read(`content/systems-programming/ch${entry.chapter}.json`);
    const topics = new Map(source.topics.map(t => [t.id, t]));
    for (const slide of deck.slides) for (const lesson of slide.sourceLessons || [])
      assert.deepEqual(lesson, topics.get(lesson.id), lesson.id);
  }
});

test('Installer is idempotent, supports both renderer versions, and preserves local edits', async () => {
  const dir = await fs.mkdtemp(path.join(os.tmpdir(), 'systems-aids-installer-'));
  try {
    for (const folder of ['scripts','src','lectures']) await fs.mkdir(path.join(dir, folder));
    for (const file of ['scripts/install-systems-discussion-aids.mjs','src/systems-discussion-aids.js','src/systems-discussion-aids.css'])
      await fs.copyFile(path.join(root,file), path.join(dir,file));
    await fs.writeFile(path.join(dir,'lectures/manifest.json'), JSON.stringify(entries));
    for (const workbench of ['function slidesWorkbench(s){','function slidesWorkbench(s) {']) {
      await fs.writeFile(path.join(dir,'src/slides.js'), `// Local edit must survive\n${workbench}return ''; }\nfunction slidesExcerpt(){}\nfunction slidesGo(){}\n`);
      await fs.writeFile(path.join(dir,'src/slides.css'), '.local-edit { color: red; }\n');
      await fs.writeFile(path.join(dir,'scripts/build-slides.mjs'), "// Local builder change\nawait import('./build-systems-slides.mjs');\nconst entries=JSON.parse('[]');\n");
      const env = {...process.env}; delete env.NODE_TEST_CONTEXT;
      const run = args => spawnSync(process.execPath, [path.join(dir,'scripts/install-systems-discussion-aids.mjs'), ...args], {encoding:'utf8', env});
      const before = await fs.readFile(path.join(dir,'src/slides.js'),'utf8');
      assert.equal(run(['--check']).status, 0);
      assert.equal(await fs.readFile(path.join(dir,'src/slides.js'),'utf8'), before);
      const first = run([]); assert.equal(first.status, 0, first.stderr);
      const installed = await fs.readFile(path.join(dir,'src/slides.js'),'utf8');
      assert.match(installed, /Local edit must survive/);
      assert.equal((installed.match(/if\(s.discussionAid\)/g)||[]).length, 1);
      const beforeSecond = (await fs.readdir(path.join(dir,'src'))).sort();
      const second = run([]); assert.equal(second.status, 0, second.stderr);
      assert.deepEqual((await fs.readdir(path.join(dir,'src'))).sort(), beforeSecond);
      assert.equal(await fs.readFile(path.join(dir,'src/slides.js'),'utf8'), installed);
      const build = await fs.readFile(path.join(dir,'scripts/build-slides.mjs'),'utf8');
      assert.match(build, /Local builder change/);
      assert.ok(build.indexOf('build-systems-slides.mjs') < build.indexOf('build-systems-discussion-aids.mjs'));
    }
  } finally { await fs.rm(dir, {recursive:true, force:true}); }
});

import test from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import crypto from 'node:crypto';
import {spawnSync} from 'node:child_process';

const root = new URL('../', import.meta.url);
const read = file => fs.readFile(new URL(file, root), 'utf8');
const json = async file => JSON.parse(await read(file));
const hash = text => crypto.createHash('sha256').update(text).digest('hex');
const entries = (await json('lectures/manifest.json')).filter(e => e.courseId === 'design-patterns-cpp');
const decks = await Promise.all(entries.map(e => json(e.path)));

test('Design Patterns discussion aids cover Class 0, every pattern, and the capstone', () => {
  assert.deepEqual(entries.map(e => Number(e.chapter)).sort((a,b)=>a-b), Array.from({length:24},(_,i)=>i));
  for (const deck of decks) {
    assert.ok(deck.designPatternsDiscussionAids, deck.title);
    for (const slide of deck.slides.filter(s => s.reading?.length && !s.code && !s.diagram && !s.visual))
      assert.ok(slide.designPatternsDiscussionAid, slide.title);
    for (const slide of deck.slides.filter(s => s.designPatternsDiscussionAid))
      assert.ok(deck.designPatternsDiscussionAids.examples[slide.designPatternsDiscussionAid.example], slide.title);
  }
});

test('Every discussion excerpt is exact delivered C++ and links to its complete original program', async () => {
  for (const deck of decks) for (const example of deck.designPatternsDiscussionAids.examples) {
    const program = deck.slides.find(s => s.id === example.fullSlideId);
    assert.ok(program?.code?.runAllowed);
    assert.equal(hash(program.code.text), example.sourceHash);
    assert.equal(await read(example.filename), program.code.text);
    assert.equal(program.code.text.split('\n').slice(example.lineStart - 1, example.lineEnd).join('\n'), example.text);
    assert.equal(program.code.expectedStdout, example.output);
  }
});

test('Discussion diagrams retain the actual chapter relationships and captions', async () => {
  for (const deck of decks) {
    const graph = deck.designPatternsDiscussionAids.graph;
    assert.match(graph.image, /^data:image\/svg\+xml;base64,/);
    const svg = Buffer.from(graph.image.split(',')[1], 'base64').toString('utf8');
    assert.match(svg, /<svg/);
    if (deck.chapter === 0) assert.equal(svg, await read('lectures/diagrams/design-patterns-ch0.svg'));
    else {
      const source = await json(`content/design-patterns-cpp/ch${deck.chapter}.json`);
      const original = source.diagrams[deck.chapter].overview;
      assert.equal(svg, await read('content/' + original.image.$asset));
      assert.equal(graph.caption, original.explanation);
    }
  }
});

test('Discussion build is deterministic and renderer supports fitted normal and modal views', async () => {
  const before = await Promise.all(entries.map(e => read(e.path)));
  const result = spawnSync(process.execPath, ['scripts/build-design-patterns-discussion-aids.mjs'], {cwd: root, encoding:'utf8'});
  assert.equal(result.status, 0, result.stderr);
  assert.deepEqual(await Promise.all(entries.map(e => read(e.path))), before);
  const css = await read('src/design-patterns-discussion-aids.css');
  assert.match(css, /\.dp-aid-figure img\s*\{[^}]*object-fit: contain/);
  assert.match(css, /\.dp-aid-viewer-image\s*\{[^}]*object-fit: contain/);
  const renderer = await read('src/slides.js');
  assert.match(renderer, /if\(s.designPatternsDiscussionAid\)return designPatternsDiscussionWorkbench\(s\);/);
  assert.match(renderer, /document.querySelector\('\.dp-aid-viewer\[open\]'\)/);
});

import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';
import {instance} from '@viz-js/viz';
import {chooseModel} from './systems-discussion-models.mjs';

const root = path.resolve(import.meta.dirname, '..');
const read = async file => JSON.parse(await fs.readFile(path.join(root, file), 'utf8'));
const digest = value => crypto.createHash('sha256').update(value).digest('hex');
export const hasMedia = s => !!(s.code || s.diagram || s.visual || s.stages?.length ||
  s.walkthrough?.length || s.sequence?.length || s.excerpt || s.excerpts?.length);

function wrap(text) {
  const lines = [''];
  for (const word of text.split(' ')) {
    if ((lines.at(-1) + ' ' + word).trim().length > 29 && lines.at(-1)) lines.push('');
    lines[lines.length - 1] = (lines.at(-1) + ' ' + word).trim();
  }
  return lines.join('\n');
}

export async function buildDiscussionAids() {
  const manifest = await read('lectures/manifest.json');
  const entries = manifest.filter(e => e.courseId === 'systems-programming');
  if (entries.length !== 61 || new Set(entries.map(e => Number(e.chapter))).size !== 61)
    throw new Error('Systems slides must be installed for all 61 chapters before adding discussion aids.');
  const viz = await instance();
  const report = {version: 1, chapters: [], totals: {pages: 0, discussionPages: 0, diagrams: 0}};
  const pending = [];
  for (const entry of entries) {
    const chapter = Number(entry.chapter);
    if (!Number.isInteger(chapter) || chapter < 1 || chapter > 61 ||
        !/^lectures\/systems-programming\/ch\d+\.json$/.test(entry.path))
      throw new Error(`Invalid Systems chapter entry: ${entry.path}`);
    const source = await read(`content/systems-programming/ch${chapter}.json`);
    const deck = await read(entry.path);
    if (deck.courseId !== 'systems-programming' || Number(deck.chapter) !== chapter || !deck.slides?.length)
      throw new Error(`Invalid deck: ${entry.path}`);
    const focus = source.diagrams[chapter].focus[0];
    const example = source.examples[chapter].examples;
    const fullCode = await fs.readFile(path.join(root, example.filename), 'utf8');
    if (fullCode !== example.code || focus.file !== example.filename ||
        fullCode.split('\n').slice(focus.lineStart - 1, focus.lineEnd).join('\n') !== focus.code)
      throw new Error(`Chapter ${chapter}: focused excerpt does not equal the delivered source.`);
    const fullSlide = deck.slides.find(s => s.code?.runAllowed && s.code.text === fullCode);
    if (!fullSlide) throw new Error(`Chapter ${chapter}: missing original runnable example slide.`);
    const graphs = {};
    let pages = 0, discussionPages = 0;
    for (const slide of deck.slides) {
      if (hasMedia(slide)) { delete slide.discussionAid; continue; }
      const model = chooseModel(chapter, slide.title);
      const highlighted = model.nodes.filter(x => x.match && new RegExp(x.match, 'i').test(slide.title)).map(x => x.id);
      const key = digest(JSON.stringify({model, highlighted})).slice(0, 16);
      if (!graphs[key]) {
        const quoted = x => JSON.stringify(wrap(x));
        const dot = `digraph discussion {
          graph [rankdir=TB, bgcolor="#ffffff", pad="0.2", nodesep="0.35", ranksep="0.70"];
          node [shape=box, style="rounded,filled", fontname="Arial", fontsize=13,
            margin="0.15,0.10", fillcolor="#f4f6f8", color="#66717a", fontcolor="#20262b"];
          edge [fontname="Arial", fontsize=10, color="#66717a", fontcolor="#354047", arrowsize=0.65];
          ${model.nodes.map(x => `${x.id} [label=${quoted(x.label)}${highlighted.includes(x.id) ? ',fillcolor="#d2f3e8",color="#167657",penwidth=2' : ''}];`).join('\n')}
          ${model.edges.map(x => `${x.from} -> ${x.to} [label=${quoted(x.label)}];`).join('\n')}
        }`;
        const svg = viz.renderString(dot, {format: 'svg'});
        graphs[key] = {title: model.title, caption: model.caption, nodes: model.nodes.map(({id, label}) => ({id, label})),
          edges: model.edges, highlighted, image: 'data:image/svg+xml;base64,' + Buffer.from(svg).toString('base64')};
      }
      slide.discussionAid = {graph: key};
      pages++;
      if (slide.reading?.length) discussionPages++;
    }
    // Shared context avoids copying a diagram or program into every page.
    deck.discussionAids = {graphs, example: {title: focus.title, filename: focus.file,
      lineStart: focus.lineStart, lineEnd: focus.lineEnd, text: focus.code, explain: focus.explain,
      invariant: focus.invariant, output: example.output, fullSlideId: fullSlide.id, sourceHash: digest(fullCode)}};
    pending.push({file: entry.path, text: JSON.stringify(deck, null, 2) + '\n'});
    report.chapters.push({chapter, pages, discussionPages, diagrams: Object.keys(graphs).length, sourceExample: example.filename});
    report.totals.pages += pages;
    report.totals.discussionPages += discussionPages;
    report.totals.diagrams += Object.keys(graphs).length;
  }
  // Validate every chapter before replacing any generated deck.
  for (const item of pending) await fs.writeFile(path.join(root, item.file), item.text);
  await fs.writeFile(path.join(root, 'docs/systems-discussion-aids-coverage.json'), JSON.stringify(report, null, 2) + '\n');
  console.log(`Added discussion diagrams and exact C++ excerpts to ${report.totals.pages} Systems pages across all 61 chapters (${report.totals.discussionPages} chapter-discussion panes).`);
  return report;
}

await buildDiscussionAids();

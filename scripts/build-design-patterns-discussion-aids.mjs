import fs from 'node:fs/promises';
import path from 'node:path';
import crypto from 'node:crypto';

const root = path.resolve(import.meta.dirname, '..');
const read = file => fs.readFile(path.join(root, file), 'utf8');
const json = async file => JSON.parse(await read(file));
const hash = text => crypto.createHash('sha256').update(text).digest('hex');

// The indices follow each chapter's authored discussion order, including its opening problem.
const discussionFocus = {
  1: [0,0,1,0,1], 2: [1,1,0,1,1], 3: [0,0,0,0,1],
  4: [1,1,0,0,1], 5: [0,0,1,0,1], 6: [1,1,0,1,0],
  7: [0,0,1,0,1], 8: [0,1,0,1,1], 9: [0,0,1,0,1],
  10: [0,0,0,1,0], 11: [0,1,0,0,0,1], 12: [0,0,0,1,1,0],
  13: [1,0,1,1,1], 14: [0,0,0,1,1], 15: [0,0,1,0,0,1],
  16: [1,0,1,0,1,1], 17: [0,0,1,1,0,1], 18: [0,0,0,0,1,0],
  19: [0,0,0,1,0,0], 20: [1,1,1,0,0], 21: [0,0,0,0,1],
  22: [0,1,0,0,1,1], 23: [2,0,0,0,1,3,4,6,5,1]
};

export async function buildDesignPatternsDiscussionAids() {
  const entries = (await json('lectures/manifest.json')).filter(e => e.courseId === 'design-patterns-cpp');
  if (entries.length !== 24 || new Set(entries.map(e => Number(e.chapter))).size !== 24)
    throw Error('Install the Design Patterns slides for Class 0 and chapters 1-23 first.');
  const pending = [], report = {version: 1, chapters: [], totals: {pages: 0, discussionPages: 0}};
  for (const entry of entries) {
    const chapter = Number(entry.chapter);
    if (!Number.isInteger(chapter) || chapter < 0 || chapter > 23 || entry.path !== `lectures/design-patterns-cpp/ch${chapter}.json`)
      throw Error(`Invalid Design Patterns lecture entry: ${entry.path}`);
    const deck = await json(entry.path), source = await json(`content/design-patterns-cpp/ch${chapter}.json`);
    if (deck.courseId !== entry.courseId || Number(deck.chapter) !== chapter || !deck.slides?.length)
      throw Error(`Invalid deck: ${entry.path}`);
    let graph, examples;
    if (chapter === 0) {
      const program = deck.slides.find(s => s.code?.filename === 'dp_ch00_replaceable_estimate.cpp');
      const visual = deck.slides.find(s => s.visual?.sourceSlideTitle === program?.title)?.visual;
      if (!program?.code?.runAllowed || !visual) throw Error('Missing Class 0 Strategy demonstration.');
      const svg = await read(visual.path);
      graph = {title: visual.title, caption: visual.caption.replace('Use horizontal scrolling at intrinsic size when needed.', ''),
        image: 'data:image/svg+xml;base64,' + Buffer.from(svg).toString('base64')};
      examples = [{title: 'One interface, two concrete rules', filename: program.code.downloadPath,
        lineStart: 5, lineEnd: 18, text: program.code.text.split('\n').slice(4,18).join('\n'),
        explain: ['Declare the shared Estimate interface.', 'Make its operations public.',
          'Allow safe destruction through the base interface.', 'State the accepted input range.',
          'Declare the pure virtual, const calculation request.', 'End the interface declaration.',
          'Declare Mean as a final implementation of Estimate.', 'Expose the override publicly.',
          'Return the integer mean for the same two inputs.', 'End Mean.',
          'Declare Highest as another final implementation.', 'Expose this override publicly.',
          'Return the greater input instead of the mean.', 'End Highest.'],
        invariant: 'The caller uses the same request while the selected concrete object supplies its behavior.',
        fullSlideId: program.id, output: program.code.expectedStdout, sourceHash: hash(program.code.text)}];
    } else {
      const bundle = source.diagrams[chapter], program = source.examples[chapter].examples;
      const fullCode = await read('companion/' + program.filename);
      if (program.code !== fullCode) throw Error(`Chapter ${chapter}: original example differs from delivered C++.`);
      const fullSlide = deck.slides.find(s => s.code?.runAllowed && s.code.text === fullCode);
      if (!fullSlide) throw Error(`Chapter ${chapter}: complete runnable source is missing.`);
      const overview = bundle.overview, asset = overview.image;
      if (!asset?.$asset || !/^assets\/[a-f0-9]+\.svg$/.test(asset.$asset)) throw Error('Invalid original diagram asset.');
      graph = {title: overview.title, caption: overview.explanation,
        image: 'data:image/svg+xml;base64,' + Buffer.from(await read('content/' + asset.$asset)).toString('base64')};
      examples = bundle.focus.map(focus => {
        if (focus.file !== program.filename || fullCode.split('\n').slice(focus.lineStart - 1, focus.lineEnd).join('\n') !== focus.code)
          throw Error(`Chapter ${chapter}: focused code does not match its source lines.`);
        return {title: focus.title, filename: 'companion/' + focus.file, lineStart: focus.lineStart, lineEnd: focus.lineEnd,
          text: focus.code, explain: focus.explain, invariant: focus.invariant,
          fullSlideId: fullSlide.id, output: program.output, sourceHash: hash(fullCode)};
      });
    }
    let pages = 0, discussionPages = 0, readingIndex = 0;
    for (const slide of deck.slides) {
      const index = slide.reading?.length ? readingIndex++ : -1;
      const media = slide.code || slide.diagram || slide.visual || slide.stages?.length || slide.walkthrough?.length || slide.sequence?.length;
      if (media || (!slide.reading?.length && (slide.excerpt || slide.excerpts?.length))) {
        delete slide.designPatternsDiscussionAid;
        continue;
      }
      const focus = chapter === 0 ? 0 : index >= 0 ? discussionFocus[chapter][index] : 0;
      if (!examples[focus]) throw Error(`Chapter ${chapter}: no reviewed excerpt for ${slide.title}`);
      slide.designPatternsDiscussionAid = {example: focus};
      pages++;
      if (slide.reading?.length) discussionPages++;
    }
    if (!discussionPages) throw Error(`Chapter ${chapter}: no discussion pages covered.`);
    deck.designPatternsDiscussionAids = {graph, examples};
    pending.push({file: entry.path, text: JSON.stringify(deck, null, 2) + '\n'});
    report.chapters.push({chapter, pages, discussionPages, examples: examples.length});
    report.totals.pages += pages;
    report.totals.discussionPages += discussionPages;
  }
  for (const item of pending) await fs.writeFile(path.join(root, item.file), item.text);
  await fs.mkdir(path.join(root, 'docs'), {recursive: true});
  await fs.writeFile(path.join(root, 'docs/design-patterns-discussion-aids-coverage.json'), JSON.stringify(report, null, 2) + '\n');
  console.log(`Added diagrams and exact C++ excerpts to ${report.totals.pages} Design Patterns pages across 24 chapters (${report.totals.discussionPages} discussion pages).`);
  return report;
}

await buildDesignPatternsDiscussionAids();

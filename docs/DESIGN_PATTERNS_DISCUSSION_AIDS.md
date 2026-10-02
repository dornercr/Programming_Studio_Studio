# Design Patterns chapter discussion aids

Adds a source-backed diagram and exact C++ excerpt to the discussion pages
for all 24 Design Patterns lectures: Class 0, chapters 1-22, and the capstone.
Other text-only lecture pages also receive the chapter context.

The diagrams are the existing, reviewed chapter diagrams, not invented class
relationships. Each excerpt is validated against consecutive lines of the
delivered C++ source. Reviewed discussion-to-excerpt mappings select the
relevant mechanism. The complete example button opens the existing editable
program slide without replacing saved drafts. Expected output is labeled as
the original expectation, not a live result. Original reading remains visible.

The normal diagram fits its available pane and viewport height without cropping
or horizontal scrolling. Enlarge opens a viewport-filling dialog with the whole
image contained. Close or Escape returns focus to the opener. The layout adapts
to narrow screens, browser zoom, and the lecture pane splitter.

## Install

Apply Design_Patterns_Chapter_Discussion_Aids.patch, then run:

```sh
node scripts/install-design-patterns-discussion-aids.mjs --check
node scripts/install-design-patterns-discussion-aids.mjs
npm run build
npm test
npm start
```

The patch only adds files. The installer checks compatibility and JavaScript
syntax before writing and makes timestamped backups of changed shared files.
It can be rerun without duplicate hooks. It preserves Systems discussion aids
and other existing lecture builders. A build hook regenerates aids after the
original Design Patterns lecture build, including offline builds.

Coverage is written to docs/design-patterns-discussion-aids-coverage.json.
For optional layout/link checks run tests/design-patterns-discussion-aids.browser.mjs
with Node; it requires Playwright and an installed Chromium browser.

# Book II discussion aids

Covers all 29 authored lecture decks: before-you-begin material, chapters 1-21,
appendices A-F, and primary references.

Every original text-only slide receives a diagram and an exact source excerpt.
Source section membership selects the corresponding listing when available;
worked discussions link the chapter's complete contract lab. Diagnostic,
vocabulary, and reference material uses relevant earlier examples. Each chapter
has its own explanatory graph, covering the ownership, type, data, or evidence
relationships actually discussed. All 84 C++ source listings remain available
as exact excerpts and downloads; 78 complete verified workshops can be edited.
Original chapter diagrams retain their delivered SVG bytes and captions.
Multi-file listings use the existing verified single-file workshop adaptation
for online execution; the original multi-file excerpts and downloads remain
unchanged and are identified separately.

Complete, verified original listings receive editable program slides only when
the same program is not already present. Nothing replaces the original slides.
All original slide fields and identities remain intact. Incomplete fragments stay
labeled as excerpts and can be downloaded; they are not offered to the compiler
as standalone programs. Added sample-driven programs initialize stdin only for
new drafts. Edited code and input remain saved independently by slide ID.

Reference observations are distinguished from deterministic sample output.
Addresses, compiler versions, and platform facts may change. Run online uses
the existing Compiler Explorer interface and requires internet access.

Normal diagrams fit their pane and viewport height without cropping. Enlarge
opens a viewport-filling dialog using contain sizing. Escape or Close restores
focus; lecture shortcuts do not change slides behind the viewer. Existing
embedded Book II diagrams use the same fitted viewer, including offline builds.

## Install

Apply Book_II_Chapter_Discussion_Aids.patch, then run:

```sh
node scripts/install-book2-discussion-aids.mjs --check
node scripts/install-book2-discussion-aids.mjs
npm run build
npm test
npm start
```

The patch adds files only. The installer validates the current renderer and
build hooks before changing them, backs up shared files, and can be rerun.
Other books and the Systems/Design Patterns/Book I aids are preserved. Do not rerun
the old authored-slides installer: that installer replaces shared app files.

Coverage: docs/book2-discussion-aids-coverage.json.
Code checks: node scripts/test-book2-discussion-aids-code.mjs.
Browser checks: node tests/book2-discussion-aids.browser.mjs (Playwright).

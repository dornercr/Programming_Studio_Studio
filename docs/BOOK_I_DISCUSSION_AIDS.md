# Book I discussion aids

Covers all 27 authored lecture decks: before-you-begin material, chapters 1-20,
appendices A-D, glossary, and technical reference directory.

Every original text-only slide receives a diagram and an exact source excerpt.
Source section membership selects the corresponding listing when available;
the checked chapter operation supplies context elsewhere. The glossary and
directory use topic-specific examples from the relevant earlier chapter.
Original chapter diagrams retain their delivered SVG bytes and captions.
Multi-file listings use the existing verified single-file workshop adaptation
for online execution; the original multi-file excerpts and downloads remain
unchanged and are identified separately.

Complete, verified original listings receive editable program slides only when
the same program is not already present. Nothing replaces the original slides.
Chapter 1 retains exactly its original 26 slides. Incomplete fragments stay
labeled as excerpts and can be downloaded; they are not offered to the compiler
as standalone programs. Added sample-driven programs initialize stdin only for
new drafts. Edited code and input remain saved independently by slide ID.

Reference observations are distinguished from deterministic sample output.
Addresses, compiler versions, and platform facts may change. Run online uses
the existing Compiler Explorer interface and requires internet access.

Normal diagrams fit their pane and viewport height without cropping. Enlarge
opens a viewport-filling dialog using contain sizing. Escape or Close restores
focus; lecture shortcuts do not change slides behind the viewer. Existing
embedded Book I diagrams use the same fitted viewer, including offline builds.

## Install

Apply Book_I_Chapter_Discussion_Aids.patch, then run:

```sh
node scripts/install-book1-discussion-aids.mjs --check
node scripts/install-book1-discussion-aids.mjs
npm run build
npm test
npm start
```

The patch adds files only. The installer validates the current renderer and
build hooks before changing them, backs up shared files, and can be rerun.
Other books and the Systems/Design Patterns aids are preserved. Do not rerun
the old authored-slides installer: that installer replaces shared app files.

Coverage: docs/book1-discussion-aids-coverage.json.
Code checks: node scripts/test-book1-discussion-aids-code.mjs.
Browser checks: node tests/book1-discussion-aids.browser.mjs (Playwright).

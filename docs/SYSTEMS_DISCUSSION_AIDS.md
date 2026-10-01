# Systems slide discussion diagrams and C++ examples

This update targets the interactive lecture pane previously labeled **The Chapter
Discussion**, across all 61 Systems Programming chapters. It also fills the
otherwise text-only question, introduction, and exercise panes. Existing code
editors, UML/source diagrams, and walkthroughs keep their existing content.

Each enhanced pane shows a chapter-specific mechanism diagram, an exact C++
excerpt from the delivered chapter demonstration, and a button that opens that
complete editable/runnable program. The original discussion remains available
under **The chapter discussion**. Topic-title matches highlight the associated
diagram nodes; pages without a specific match explicitly label the figure as
chapter context. A shared chapter example is not presented as a new independent
program for every topic.

The diagrams are new explanatory models, not screenshots of the original
presentations or measurements of real hardware. For example, page translation,
cache indexing, signal coalescing, pointer bounds, process states, and concurrent
ownership have different explicit relationship graphs. Chapter 1 and Chapter 2
select additional diagrams for their different mechanism families. Captions
state modeling limits and assumptions. C++ code, line ranges, and expected output
are checked against actual packaged source; unsafe memory-access examples are
not introduced.

## Install

Prerequisite: the working Systems slide layer for all 61 chapters is already
installed. This patch does not reinstall or replace the earlier slide patches.

```bash
cd "$HOME/Downloads/Design_Patterns_Study_Studio_Source/Design_Patterns_Study_Studio" &&
git apply --check "$HOME/Documents/Systems_Chapter_Discussion_Aids.patch" &&
git apply "$HOME/Documents/Systems_Chapter_Discussion_Aids.patch" &&
node scripts/install-systems-discussion-aids.mjs &&
npm run build &&
npm test &&
npm start
```

The patch adds new files only, avoiding conflicts with previous edits to the
shared renderer or build script. The installer validates compatibility and
JavaScript syntax before editing `src/slides.js`, `src/slides.css`, and
`scripts/build-slides.mjs`. It creates timestamped backups of those three files
and inserts only the aid-rendering/build hooks. It does not change `src/app.js`,
routes, course switching, or other books. Running the installer again is safe.
After an installation, use `npm run build` normally; discussion aids are
regenerated after the Systems lectures and before deployment/offline embedding.
The build integration also excludes a self-reference from the generated integrity
manifest: a manifest cannot contain a valid hash of its own final contents.

If the patch is already applied, skip the two `git apply` commands and start
with `node scripts/install-systems-discussion-aids.mjs`.

Open the site normally, select Systems, choose any chapter, and open Lectures.
Use a discussion/concept page. The right pane contains the diagram and C++
excerpt. **Edit and run the complete example** navigates to the existing
chapter program without replacing any saved code drafts. Refresh after rebuilding
to load the updated application and chapter shard.

## Maintained source

- `scripts/systems-discussion-models.mjs`: explicit diagram relationships.
- `scripts/build-systems-discussion-aids.mjs`: verifies source and decorates the
  generated chapter shards, without rewriting authoritative lesson objects.
- `src/systems-discussion-aids.js` and `.css`: aid-rendering source templates.
- `scripts/install-systems-discussion-aids.mjs`: idempotent integration installer.
- `tests/systems-discussion-aids.test.mjs`: coverage, exact C++, retained source,
  diagram references, and installer preservation/idempotence checks.
- `docs/systems-discussion-aids-coverage.json`: generated coverage counts.

The existing locked `@viz-js/viz` dependency lays out the SVG diagrams. No new
dependency, external image service, or network request is needed to view them.
Diagrams are embedded in their lazy-loaded chapter shard, so deployment prefixes
and the offline build do not need new asset-path rules. Running the complete
example online still uses the project's existing remote compiler service.

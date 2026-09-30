# Design Patterns study expansion and editorial correction

This patch applies on top of the Book I study expansion. It changes Design Patterns and the small shared rendering/build paths needed for its study tools. It does not publish or deploy the project.

## What was confusing and what changed

- Every guided lesson previously reprinted its complete chapter workshop and usually its chapter discussion. Design Patterns lessons now show their own prose/code and link to the canonical chapter-map workshop. No workshop content was discarded.
- Overview text referred to unrelated transport, furniture or EPUB examples. Active introductions now connect the pattern intent to the delivered C++ program. The exact previous wording is retained in the editorial ledger.
- Repeated generic role, trace and failure prompts now name the chapter's actual responsibilities, initial state and invariant. Scenario lessons no longer repeat the instruction to open another tab; the choices and all choice rationales are available in place.
- The older Observer mini-lab uses a listener snapshot. The chapter Feed rejects subscription changes during delivery. Both remain available, with their different contracts clearly labeled. Independent pricing and renderer mini-labs are labeled too.
- The capstone's guarded enum state machine is explicitly distinguished from the polymorphic State pattern.
- Workshop output is labeled as the output of the **complete program**, rather than implying that an adjacent excerpt alone prints it.
- The first problem lesson and final discussion in each chapter have contextual questions. Earlier explanations no longer inherit an unrelated repeated discussion automatically.

## Counts

| Design Patterns material | Before | After |
|---|---:|---:|
| Chapters, including Class 0 | 24 | 24 |
| Reading entries | 539 | 539 |
| Flashcards | 609 | 1,009 |
| Diagram overview views | 25 | 47 |
| Existing focused code/theory panels | 51 | 51 |
| Complete original C++ resources | 69 | 69 |
| Graded coding questions | 3 | 50 |
| Public coding checks | 12 | 388 |
| Registered checked worked programs | 0 | 69 |

Each of the 22 patterns gets 17 new cards, two distinct repair challenges, and a complementary activity view. The capstone gets 26 new cards and three integration repairs. The 376 added check executions use 184 distinct behavioral probes across the 47 repairs; related repairs deliberately share regression checks. The 400 cards comprise 184 executable traces, 44 defect-diagnosis cards and 172 design/maintenance questions. Trace cards identify the extended implementation and link to it. They are expressions, not standalone programs.

The 22 new diagrams quote consecutive source ranges from the original `main` demonstration. Solid arrows mean execution order between excerpts, not a complete control-flow graph. Dashed links identify teaching notes, not UML dependencies. Adjacent panels include invariants, actual chapter failure cases and exact filenames/lines. The original class, ownership, sequence, state and other views remain intact. Wide diagrams scroll horizontally instead of shrinking code on phones.

## Source and preservation

- `content/design-patterns-expansion.json`: authored additions plus an explicit before/after field ledger.
- `scripts/design_patterns_expansion/`: original behavioral probes, compiling defects, recall questions and problem discussions.
- `scripts/build_design_patterns_expansion.py`: creates the authored pack and complete starter/solution resources.
- `scripts/render-design-patterns-expansion.mjs`: emits editable Graphviz sources and SVG assets.
- `scripts/design-patterns-expansion.mjs --apply`: idempotently applies the reviewed pack to modular source. Unexpected edited-field drift is an error, not an overwrite.
- `coding_lab/design_patterns_expansion/chNN/`: complete C++20 repair starters and solutions, including the original extension demonstration driver.
- `companion/`: all 69 original programs and their recorded outputs remain byte-identical.

All existing topic, card and question IDs remain. No localStorage schema changes are needed. Notes, bookmarks, ratings, progress, coding drafts and backup/import keep the existing version-1 format. Previously imported free-playground drafts remain stored; opening the Code view now selects the registered worked-program entry with its own independent draft.

Historical hash tests reconstruct the prior content by reversing only the known editorial edits and removing only identified additions. They reject arbitrary drift. The build still compares the complete current modular source with generated output. All 3,249 original educational resource files retain their original hashes. Other courses, including Book I, retain their source content.

## Apply either delivery form

From the project root, use the patch:

```sh
git apply --check /absolute/path/Design_Patterns_Study_Expansion.patch
git apply /absolute/path/Design_Patterns_Study_Expansion.patch
npm ci
npm run build
npm test
```

Or copy the contents of the ZIP's `changed-code/` directory into the project root, preserving paths, then run the same npm commands. Use **one** method. The patch is based on the post-Book-I project; `git apply --check` detects a different baseline without changing files. Generated `dist/` is rebuilt locally and is not duplicated in this changed-code delivery. Deployment remains the existing GitHub Pages workflow publishing `dist/`.

Review and commit in your actual Git working tree:

```sh
git diff --stat
git status --short
git add README.md package.json src scripts tests docs content coding_lab/design_patterns_expansion diagrams/design_patterns_expansion
git commit -m "Improve Design Patterns lessons and expand verified study practice"
```

Review unrelated local edits before staging those directories.

## Rebuild and verify

Normal build uses the authoritative modular files and needs no Python or C++ compiler:

```sh
npm ci
npm run build
npm test
npm run validate
```

To regenerate this pack from its authored specifications with Python 3 and Node:

```sh
python3 scripts/build_design_patterns_expansion.py
node scripts/render-design-patterns-expansion.mjs
node scripts/design-patterns-expansion.mjs --apply
npm run build
```

These commands reproduce the supplied pack from the unchanged specifications. If intentionally editing wording already covered by the ledger, review and restore the previous field values before applying a newly authored ledger; the applier deliberately rejects unknown intermediate wording. `npm run content:build` and `npm run coding:build` reapply this reviewed pack after the legacy authoring generators, just as they reapply Book I's pack.

```sh
npm run test:design-patterns-expansion
npx playwright install chromium
npm run test:design-patterns-expansion-browser
# Or: CHROMIUM_PATH=/absolute/path/chromium npm run test:design-patterns-expansion-browser
```

The first command compiles every reference, sample driver, broken starter and registered original program with g++ C++20. It compares samples exactly, checks every expected result, and requires each starter to compile and fail at least one public check. Original resources retain their C++17 instructions and also compile as C++20 in this suite.

The browser suite runs beneath `/Programming_Studio_Studio/`. It checks focused reading, chapter workshops, all new lab guides, failing/passing feedback with a real local compiler, cold Code-to-lab navigation, return-to-source variants, refresh, all added diagram assets, flashcard links, notes/bookmarks/ratings backup and restore, and desktop/mobile overflow. It blocks real external submissions; remote compiler availability is not claimed. Automated checks do not prove every possible input or concurrent execution; each exercise states its own scope.

Results are in `tests/design-patterns-expansion-verification.json`, `tests/design-patterns-expansion-browser-results.json` and `docs/DESIGN_PATTERNS_EXPANSION_COUNTS.json`. The delivery ZIP also includes visual QA captures and the clean-application verification report.

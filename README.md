# Programming Studio — modular edition (2.1)

Ten complete curricula by **Dr. Charles Dorner**: Design Patterns, Systems Programming and Machine Organization, and eight C++ textbooks. The lossless modular architecture is retained. Version 2.1 adds the Book III coding expansion without changing any original educational content, source books, lectures, examples, diagrams, earlier coding exercises or study functions.

## New: Book III worked examples and coding practice

Open **Book III · Data Structures and Algorithms in C++ → Coding Lab**. There are **66 runnable workshops covering all 68 C++ listings**, with 71 recorded behavior cases, and **22 independent exercises with 116 checks**. Exercises cover every core chapter and the two executable appendices. Problem, design/alternative, invariant, concrete trace, failure and maintenance discussions sit next to the editor. Original source and clearly labeled single-file adaptations stay separate. All previous books are unchanged.

```sh
npm run test:book-three         # local GCC C++20 tests; no network
npm run test:book-three-browser # local server + Chromium; no external code submission
npm run package:book-three      # standalone companion ZIP after verification
```

The standalone companion includes a study guide, separate explained answer key, every original Book III listing, starter/solution/driver files, all workshops, and the original multi-file HarborRoutes project. Its `node verify.mjs` needs only Node and g++, not npm dependencies or internet. See [BOOK_III.md](docs/BOOK_III.md). The full library now has 90 coding questions with 425 checks and 230 source-linked runnable workshops across Books I–III; the rest of the library is preserved.

## Build and run

Use Node.js **22 LTS** (minimum 20.11) and npm. The normal build needs no Python, compiler, remote API or backend.

```sh
npm ci
npm run build
npm start
```

Open `http://127.0.0.1:5173/`. Do not open the modular `dist/index.html` directly with `file://`; browsers restrict JSON/module fetches there. Use the offline build below for a double-clickable file.

To test the exact Pages project path:

```sh
BASE_PATH=/Programming_Studio_Studio/ npm start
```

Open `http://127.0.0.1:5173/Programming_Studio_Studio/?course=cpp-book-03&chapter=17&view=code`.

## Deploy to GitHub Pages

Publish **`./dist`**, including its subdirectories and `.nojekyll`. The supplied `.github/workflows/pages.yml` builds, runs integrity tests and publishes that directory on a push to `main` or a manual dispatch. Choose **GitHub Actions** as the repository's Pages source. No domain-root routes are used.

The recovered project archive contained no GitHub workflow, so this release includes a workflow using the requested `./dist` deployment model. No remote repository or deployed website was changed during this refactor.

Target: `https://dornercr.github.io/Programming_Studio_Studio/`.

## What loads

- Startup: HTML, CSS, two small JavaScript files, the master catalog, the selected book's navigation metadata and one chapter.
- A chapter opens its own JSON. Already loaded chapters stay in memory for the session.
- Search fetches compressed, search-only shards for **the selected book**, on the first nonempty query. It retains the original full substring search, including searchable code. It does not download other books or the chapter library.
- The Coding Lab fetches the selected book's questions/worked programs when opened.
- Diagrams, original PDFs/EPUBs and lecture presentations use separate relative asset URLs.
- Exporting a complete study pack is an explicit exception: it loads that book and restores embedded assets into the compatible export format.

## Source layout

```text
content/manifest.json            source order and format
content/<course>/source.json     course-level source fields
content/<course>/ch<number>.json complete chapter content
content/<course>/coding.json     questions and runnable workshops
content/assets/                 losslessly extracted binary assets
src/                            existing UI + lazy loader + styles
scripts/                        build, migration, validators and tests
companion/, cpp_series/, ...     original code, source books and teaching inputs
```

`content/` is authoritative for normal builds. `src/content.json` and `src/coding-content.json` are no longer required or tracked. Source topic IDs, block IDs, filenames, line references and data strings are unchanged. Details and extension steps are in [ARCHITECTURE.md](docs/ARCHITECTURE.md).

Original authoring generators remain available through a compatibility bridge:

```sh
npm run content:build   # Python 3 plus the installed @viz-js/viz package
npm run coding:build    # rebuild only the coding catalog
npm run build
```

These commands temporarily materialize the old source format, run the original generators, convert their complete output back into chapter files and remove the temporary monoliths. They are **editorial regeneration commands**, not necessary for deployment. Review their changes before committing; do not use them merely to rebuild the current approved content.

## State and migration

The storage key remains `patterns-study-studio:v1`, with stable course/topic/card/practice IDs and backup version 1. No destructive migration or storage reset is performed. Valid progress entries are no longer silently limited by collection-size slices as the library grows.

A change from a local file to an HTTPS website changes the browser's storage origin. Export a backup from the old app, open the new app, then use **Settings → Import / restore**. Moving to the same hostname and project path preserves existing browser storage. Keep a backup before changing browsers or clearing site data.

Reading position, notes, bookmarks, card ratings, review queues, practice history, coding drafts/checks, custom material and preferences remain local. The loader does not upload them. Coding Lab still sends the current code/input to Compiler Explorer only after the student explicitly selects an online run/check.

## Offline edition

```sh
npm run build:offline
```

Open `dist-offline/index.html` directly. This optional self-contained edition embeds all reading material and original downloads. It uses the same UI and state/export format. Online C++ execution still requires a connection. `dist-offline/` is ignored by Git and is not part of the Pages deployment.

The modular edition keeps loaded chapters in memory during the current session. It does not promise offline refresh or uncached chapter access; use the offline edition for complete offline reading.

## Verification

```sh
npm test                       # original content tests + exact migration/integrity tests
npm run validate               # compare current modular source with generated output
npx playwright install chromium
npm run test:browser            # starts its own server under the Pages subdirectory
```

For a preinstalled Chromium, set `CHROMIUM_PATH=/absolute/path/to/chromium`. The browser suite visits every chapter/reference group and all study modes, tests saved state and backup/import, downloads, search, mobile layout, deep links and network isolation. See [MIGRATION.md](docs/MIGRATION.md), [SIZE_REPORT.md](docs/migration/SIZE_REPORT.md), and the machine-readable results in `tests/`.

The unchanged content suites verify all source listings, original book/presentation bytes, diagram relationships and exact line excerpts. Existing specialized browser suites are retained and default to the offline build; `STUDIO_URL` directs them to an HTTP deployment. Run `npm run build:offline` first for those legacy suites.

C++ sources keep their original standards and requirements: Design Patterns/workshops generally C++17; the eight-book series and Coding Lab generally C++20; some advanced samples require CUDA, third-party dependencies, services or hardware. Read each original build instruction and verification record. No claim is made that those prerequisites were added by this refactor.

## Release files

- Complete project ZIP: editable source, build scripts, original resources, tests, docs and generated `dist/`.
- Pages ZIP: the contents of `dist/`, ready for static hosting.
- Offline ZIP: self-contained HTML plus instructions.

The earlier teaching and C++ verification notes remain in `docs/README-v1.7.md`, `coding_lab/README.md`, and other original documentation, preserved byte-for-byte as historical resources. Their earlier counts and single-file build instructions describe those releases; use this README and `docs/BOOK_III.md` for version 2.1.

## Expanded Book I study pack

Book I now includes 519 flashcards, 25 UML/diagram views, and 47 Coding Lab challenges with 236 public checks, alongside all 86 existing runnable workshops. The additions cover every core chapter and four practical appendices. See [Book I study expansion](docs/BOOK_I_STUDY_EXPANSION.md) for exact counts, authoring commands, preservation checks, browser verification, and patch/commit instructions.

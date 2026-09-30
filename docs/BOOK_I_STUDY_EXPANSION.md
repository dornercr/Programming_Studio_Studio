# Book I study expansion

This additive pack covers all 20 core chapters of **C++ Foundations** and four practical appendices. It preserves original reading, exercises, source programs, diagrams, card IDs, and version-1 browser study data. It does not change any other course's curriculum.

| Book I material | Before | After |
|---|---:|---:|
| Flashcards | 119 | 519 |
| Diagram overview views | 1 | 25 |
| Chapters/appendices with diagram studies | 1 | 24 |
| Focused code/theory panels | 2 | 26 |
| Coding Lab practice challenges | 23 | 47 |
| Public challenge checks | 108 | 236 |
| Source-linked runnable workshops | 86 | 86 |
| Reading entries | 575 | 575 |
| Original source listings | 136 | 136 |

There are 18 new cards per core chapter and 10 per practical appendix. The 400 additions include recall, syntax, boundaries, code-output prediction, actual starter defects, invariants, design choices, and explained alternatives. Code cards show the function/class and driver; standard headers are assumed, and the supplied input is explicit. Their sample answers are verified against compiled solutions.

The 24 new challenges are independent of the 86 original workshops. Each includes a clear contract, intentionally defective but compilable starter, reference, driver, sample, progressive hints, at least four checks, and a design/invariant/trace/failure/maintenance discussion. Chapter 19 also supplies a real header plus two separately compiled source files.

Diagram studies include activity flows, Stock and Interval class relationships, Parcel state transitions, sole array ownership, and a construction-failure sequence. Code excerpts match actual solutions. Every focus panel references an exact file and line range. The original Chapter 1 study remains alongside the addition. A diagram's matching challenge and each lab card link directly into Coding Lab.

## Architecture and authoring

- `content/book-one-expansion.json` is the editable addition pack.
- `scripts/book_one_expansion/recall.txt` contains 256 authored recall cards.
- `scripts/book_one_expansion/labs.py` contains the 24 authored challenges and their teaching contracts. Six associated cards per challenge supply the remaining 144 cards.
- `scripts/build_book_one_expansion.py` regenerates the pack and runnable companion files.
- `scripts/render-book-one-expansion.mjs` renders SVG assets and editable Graphviz diagram sources.
- `scripts/book-one-expansion.mjs --apply` adds the pack to modular chapter/coding files idempotently.
- `content:build` and `coding:build` reapply this pack after the existing authoring generators run.
- Normal `npm run build` reads the modular files and copies chapter content/assets as before. It does not regenerate or download all books at browser startup.

To edit/rebuild this pack, run from the repository root:

```sh
python3 scripts/build_book_one_expansion.py
node scripts/render-book-one-expansion.mjs
node scripts/book-one-expansion.mjs --apply
npm run build
npm test
node scripts/test-book-one-expansion.mjs
```

Requirements: the existing Node.js >=20.11/npm environment (Node 22 or newer recommended), Python 3.10+, and the existing pinned npm dependencies. C++ execution requires a C++20 compiler; verification used GCC 13.3. The online lab continues to use the existing explicit Compiler Explorer Run/Check actions. Merely reading, editing, or opening a view sends no code to that service.

For browser checks:

```sh
# Install the matching Playwright Chromium once if it is not installed.
npx playwright install chromium
node scripts/book-one-expansion-browser.mjs
# Or supply an existing compatible Chromium:
CHROMIUM_PATH=/absolute/path/to/chromium node scripts/book-one-expansion-browser.mjs
```

The browser test serves `/Programming_Studio_Studio/`, intercepts compiler requests, and runs those C++ submissions locally. It makes no real external code submission. It verifies card flipping, old/new ratings, review queue, code-card links, all added challenge/diagram reachability, SVG loading, large view, real starter/reference feedback, draft restoration, backup/import, unaffected Book II, and 390-pixel layout.

## Preservation tests

Historical hashes are retained, not replaced. `withoutBookOneExpansion` removes only the 400 named cards, 24 named diagram studies, and their derived card count to reconstruct the exact earlier educational source. `withoutBookOneQuestions` filters only the 24 named new challenge IDs. Existing hash tests still require every prior content field and coding item to match. New tests require every added card, challenge, and focus line to match the authored pack. Source-to-dist hydration and all resource checks remain active.

## Apply and commit

This patch targets the current modular Programming Studio 2.1.0 source with the Book III workshops. Apply from its repository root; review `git status` first so your local work is visible.

```sh
git apply --check /path/to/Book_I_Study_Expansion.patch
git apply /path/to/Book_I_Study_Expansion.patch
npm ci
npm run build
npm test
node scripts/test-book-one-expansion.mjs
git status --short
git diff --stat
```

Review and stage the changed source files, then commit. Generated `dist/` is intentionally omitted from this change package: `npm run build` regenerates it, and the existing GitHub Pages workflow continues to publish `./dist`. If your repository tracks `dist/`, stage its regenerated files as part of your commit. If `git apply --check` reports a mismatch, do not force it; the `changed-files/` copy provides the exact edited/new files for comparison and manual merging.

Existing card identifiers and the local-storage schema stay unchanged, so original ratings, notes, bookmarks, practice history, and coding drafts do not require a migration. New items simply start without ratings/results.

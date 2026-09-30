# Book III — Data Structures and Algorithms in C++

Author: Dr. Charles Dorner. Programming Studio 2.1 adds this book’s coding expansion only. No GitHub repository or live Pages site is changed by this release.

## Coverage

| Measure | Before | After |
|---|---:|---:|
| Core chapters | 20 | 20 |
| Declared chapter/front/reference groups | 26 | 26 |
| Reading/study entries | 479 | 479 |
| Original listings (all languages) | 96 | 96 |
| Original C++ listings | 68 | 68 |
| Source-linked runnable workshops | 0 | 66 |
| Workshop behavior cases | 0 | 71 |
| Independent coding exercises | 3 | 22 |
| Public exercise checks | 14 | 116 |

Every original listing is retained. Two supporting HarborRoutes files are included in both runnable project/test bodies, so 68 C++ listings become 66 executables rather than 68 independent programs. The shared CHECK header is copied unchanged from Book II B02-L0110 and labeled as support, not counted as a new Book III listing. `tests/book-three-verification.json` gives the actual build/results record. These programs execute 2,228,631 internal CHECK assertions in addition to the 71 outer input/output/exit cases; that is an execution count, not that many unique tests or exhaustive proof.

## What was added

The Coding Lab offers source-linked programs grouped by chapter, exact recorded input/output, original-source links, and an editable complete C++20 program. Beside the editor, the discussion explains the chapter problem, the selected design and alternative, its invariant, a concrete trace, possible failure, and maintenance consequences. Short source demonstrations and larger invariant labs are explicitly distinguished.

Independent exercises cover first-maximum ties, operation counting, indexed deletion, linked ownership, brackets, circular FIFO reuse, hashing/order, frequency tables, tree height, BST insertion, rotations, heap removal, graph representation, BFS, binary search, stable sorting, backtracking, greedy interval selection, dynamic programming, matrix validation, path witnesses, and transactional route parsing. Every starter compiles but fails a stated check. References preserve empty/boundary/error behavior, and explanations discuss plausible alternatives. `test:book-three` also executes each broken starter’s sample, checking the incorrect output discussed in the explanation.

The standalone companion adds `STUDY_GUIDE.md` and a separate `ANSWER_KEY.md`, with chapter-level discussion, executable examples, progressive experiment/extension prompts, hints, actual incorrect sample output, and explained reference implementations. `projects/harbor-routes/` restores the original routes.hpp/routes.cpp/main.cpp/tests.cpp filenames and documents the real multi-file build.

## Preserve and adapt

`coding_lab/book_03_worked/<listing-id>/original.cpp` is the original listing, including programs that need shared files. `main.cpp` is the runnable original or labeled adapter. `workshop.json` identifies source parts and assumptions. Local include lines are removed only when their exact source is placed before the program. No algorithm body is changed merely to force a compilation pass.

Three demonstrations have portable teaching displays:

- B03-L0012: element offsets rather than integer-address byte offsets.
- B03-L0029: the equal-key hash guarantee rather than a library-specific numeric hash.
- B03-L0083: the nonnegative clock-duration check rather than a changing nanosecond median.

The exact originals remain available for byte-layout, numeric-hash and real-timing study. Benchmark mode in B03-L0084 is also preserved; the default verification path is deterministic. No passing timing check promises a particular speed or cross-platform performance result.

## Rebuild and verify

Normal web build: Node >=20.11 (22 LTS recommended), npm; no compiler needed. C++ verification: Linux, GCC with C++20 support, standard library, `-pthread`. Delivered local checks used GCC 13.3.0.

```sh
npm ci
npm run build
npm test
npm run test:book-three
npx playwright install chromium
npm run test:book-three-browser
npm run test:modular-browser
python3 scripts/package_book_three.py /absolute/output/Book_III_CPP_Coding_Workshops.zip
```

If Chromium is already installed, set `CHROMIUM_PATH` to its absolute executable path. Browser tests bind a local server; they need an environment that permits localhost. The Book III suite intercepts the compiler request and executes that exact code through local g++; no code is submitted externally. The full regression suite checks every chapter group, study mode, deep links, search, diagrams, download paths, state, backup/restore and mobile layout beneath `/Programming_Studio_Studio/`.

For editorial regeneration, use `npm run coding:build`, then `npm run build`. The bridge temporarily recreates the old source format, adds Books I–III coding definitions, and returns the result to modular files; the original migration baseline remains immutable. `scripts/book_three_baselines.json` contains reviewed expected results. `scripts/probe_book_three.py` can recapture fixtures for an explicitly reviewed adapter change; normal verification never rewrites the expected results.

## Integrity and limits

`docs/book-three-baseline.json` records every prior coding item’s SHA-256. Tests require exact original educational content and exact prior questions/workshops; only the 19 new questions and 66 new workshops are additive. The original 3,249 resource files are still verified byte-for-byte, including their historical README versions. IDs and the version-1 local study storage/backup format do not change.

All original reading, flashcards, practice, diagrams, books, downloads, notes and other study tools remain. Book I still has 23 questions/86 workshops; Book II has 24/78. Lazy loading still fetches only the requested book/chapter data. Original PDF/EPUB bytes are not edited: the new exercises are Studio/companion additions, not a rewritten textbook edition. Online execution in the app still requires internet and a user-triggered compiler request; the companion tests work offline.

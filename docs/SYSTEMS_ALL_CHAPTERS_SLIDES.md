# Systems Programming: interactive lectures for all 61 chapters

This patch extends the same interactive Slides format used in Design Patterns and the supplied C++ Book I Chapter 1 page to **every Systems Programming and Machine Organization chapter, 1–61**. The author credit is **Dr. Charles Dorner**.

It adds 2,751 interactive lecture pages, including 614 newly authored teaching pages. Each chapter has at least two new complete C++ demonstrations, spoken presenter narration, output explanations, questions with explained answers, the original complete example and lab, six progressive exercises, an implementation diagram, and a focused source view with line-by-line explanations and a linked theory panel. Chapter 61 integrates the course in a longer capstone.

The 305 complete runnable program views comprise **122 new lecture demonstrations** plus **183 existing chapter programs, lab starters, and lab solutions**. These are available in Slides; this patch does not claim to add 305 separate Coding Lab questions. All **130 diagram/source-focus views** reuse the actual existing Systems diagrams and source blocks; they are not 130 newly drawn UML assets. The diagram type follows each chapter's mechanism, including execution flow and ownership where appropriate.

## Content coverage and repeated source material

All **2,690 original Systems reading entries** from chapters 1–61 are preserved exactly as source objects inside the lecture records. They include all **1,775 supplied presentation-slide records**, their original text and professor transcripts, the 854 detailed workshop/exercise lessons, and 61 application questions. The separate glossary, source downloads, original Outline, flashcards, practice, labs, notes, bookmarks, and progress remain in the application.

Repeated source “Deep Dive” records are grouped under the corresponding concept rather than forcing learners through identical lecture pages. Expand **Original source lessons covered here** to see every original record and full transcript, or use **Open this lesson in Outline**. Grouping does not delete or rewrite the authoritative source. Tests compare each retained source object with its original chapter object, including all nested fields.

A known historical description in Chapter 45 models a lost update in an ordinary shared counter. The existing correction is displayed prominently: an actual unsynchronized ordinary C++ counter has a data race and undefined behavior. Its result is not guaranteed to be one. New demonstrations use defined operations or explicit deterministic models. Original wording remains available in the source panel with the correction beside it.

## Find and use the lectures

Open **Systems Programming and Machine Organization → choose any chapter → Slides**. The lecture chapter menu contains all 61 chapters. The slide menu groups pages into problem, mechanisms, code/diagrams/debugging, practice, and recall sections.

Examples after `npm start`:

- `http://localhost:5173/?course=systems-programming&chapter=1&view=slides&slide=1`
- `http://localhost:5173/?course=systems-programming&chapter=21&view=slides&slide=1`
- `http://localhost:5173/?course=systems-programming&chapter=45&view=slides&slide=1`
- `http://localhost:5173/?course=systems-programming&chapter=61&view=slides&slide=1`

The `slide` parameter is one-based. Refresh restores the requested chapter and slide. The deployed form retains the project prefix:

```text
https://dornercr.github.io/Programming_Studio_Studio/?course=systems-programming&chapter=61&view=slides&slide=1
```

Use **Presenter script and directions** for the complete SAY narration and DO directions. **Download transcript** exports the current chapter script. Downloadable generated scripts are also in `lectures/transcripts/systems-programming/`.

Predict the printed values, run a complete example, and connect each output line to the operations described in the narration. The expected-output panel is explicitly the original example's expectation. It is not a claimed result of edited code. Edited code can be reset, downloaded, or exported with the chapter's lecture work. A previous live result is marked stale after an edit.

The new examples distinguish **Portable C++20** from **Linux/POSIX**. Explicit models explain rules without claiming to measure a real processor, MMU, cache, kernel, or network. POSIX examples use real local mechanisms such as child processes, descriptors, mappings, pipes, signals, or socket pairs. They do not require an outside server. Runtime resource failures may produce diagnostics instead of the documented normal-success output.

## Apply the patch

This is an incremental patch for the project with **Design_Patterns_All_Chapters_Slides.patch** already applied, including its shared Book I Slides feature. It preserves those lectures and their authoring files. A separate, newer local modification to the same shared renderer or manifest may require reconciliation; the check below detects that before changing files.

Download `Systems_All_Chapters_Slides.patch`, then run:

```bash
cd "/home/charles/Documents/Design_Patterns_Study_Studio_Source (2)/Design_Patterns_Study_Studio" &&
git apply --check "$HOME/Downloads/Systems_All_Chapters_Slides.patch" &&
git apply "$HOME/Downloads/Systems_All_Chapters_Slides.patch" &&
npm run build &&
npm test &&
npm start
```

`git apply` also works in the extracted project folder without a `.git` directory. Do not force the patch if the check fails. On a fresh dependency installation, run `npm ci` before building. `npm start` serves the generated site at `http://localhost:5173/` until Ctrl+C. After checking it, use the existing rsync and commit workflow for the actual Git working tree. Applying a patch does not deploy the hosted site. GitHub Pages should continue publishing `./dist`.

The changed-code ZIP contains the patch, edited/new source files, generated lecture shards, C++ downloads, transcripts, tests and results, and this guide. It is an incremental update, not a replacement for the project's existing educational library. Rebuild to regenerate the complete modular `dist/`.

## Build architecture and editable source

| Path | Role |
|---|---|
| `lectures/authoring/systems-programming/chN.json` | Editable new teaching, narration, questions, complete demonstrations, and expected output. |
| `content/systems-programming/chN.json` | Existing authoritative chapter lessons, original source transcripts, examples, and diagram references. |
| `systems_examples/chNN/main.cpp` | Existing complete chapter demonstration. |
| `systems_labs/chNN/starter.cpp`, `solution.cpp` | Existing lab starter and reference solution. |
| `systems_source/Chapter_NN/` | Existing original presentation and transcript downloads. |
| `scripts/build-systems-slides.mjs` | Deterministically generates lecture shards, transcripts, downloads, coverage counts, and source hashes. |
| `lectures/systems-programming/chN.json` | Generated chapter lecture; fetched only when required. |
| `lectures/examples/systems-programming/chN/` | Complete generated C++ downloads, matching the editors exactly. |
| `lectures/transcripts/systems-programming/chN.txt` | Generated chapter SAY/DO scripts. |
| `lectures/systems-source-integrity.json` | SHA-256 hashes of the 366 existing source inputs used directly. |
| `lectures/manifest.json` | Lightweight lecture catalog, retaining the other courses. |
| `src/slides.js`, `src/slides.css` | Shared interactive UI, source archive, model/platform labels, and responsive layout. |
| `scripts/build-slides.mjs` | Runs lecture generation and the normal modular/offline builds. |

Edit the authoring or authoritative source, then use `npm run build`. Do not maintain separate manual versions of generated lecture JSON or `dist/`. The normal build keeps complete lecture content outside the application JS. Opening Outline loads no lecture shard; opening Slides fetches just the requested chapter's JSON. Previously loaded decks are cached in memory. Relative URLs work beneath `/Programming_Studio_Studio/`.

Requirements: Node.js 20.11+, npm with the existing lockfile, a C++20 compiler for local program checks, and Chromium/Playwright for browser tests. The verification machine used GCC 13.3 on Linux. The POSIX examples require Linux or a compatible POSIX environment; use WSL on Windows. The browser's C++17 selector remains available for experiments, but these additions are authored and verified as C++20.

## State, backup, and online execution

The existing reading and study IDs remain unchanged. Lecture position and code drafts use the existing `patterns-study-studio:slides:v1` storage, separately for each course/chapter/slide. No existing storage migration is required. New Systems code-slide IDs derive from the source filename or path, so inserting another page earlier in a lecture does not attach a saved draft to a different program.

**Export lecture work** backs up the current lecture's position and drafts. Import it with the matching course/chapter open. The general study backup still serves the original study records; lecture drafts have their own export/import and should be backed up separately.

**Run online** explicitly submits the current source and stdin to the configured Compiler Explorer endpoint. It needs internet and a functioning external service, and it is not a local shell. Restricted online sandboxes may reject POSIX facilities such as process creation. Download the `.cpp` and use its displayed local build command on Linux if needed. Reading, editing, output inspection, and downloading do not submit code.

Local verification and browser tests do not establish the external service's live availability. Browser QA intercepts compiler requests and invokes the local compiler; it sends no source to the public compiler service. Stop waiting cancels the browser wait, not necessarily work already submitted remotely.

## Optional offline build

```bash
npm run build:offline
```

This creates the separate `dist-offline/` distribution with the lecture data included. Keep that directory and its accompanying resources together. Offline reading, diagrams, editing, and draft restoration work without network requests where browser storage is available. Running downloaded code requires a local compiler; the online Run button still needs internet. The web deployment continues to use the modular `dist/`.

## Reproduce validation

```bash
npm run build &&
npm test &&
node scripts/test-systems-slides-code.mjs &&
node tests/systems-slides.browser.mjs
```

The C++ test builds and runs every complete Systems lecture program, checking exact stdout, stderr, and exit status. It uses `g++` and four parallel jobs by default. Use `CXX` and `CPP_JOBS` to select a compatible compiler or reduce concurrency.

The content tests verify all 61 chapter entries, every original source object, input hashes, authored narration, runnable examples, diagram selectors and assets, exact source line references, complete C++ download equality, transcripts, and lazy-loading structure. They also require the Chapter 45 correction.

The browser suite visits every generated Systems slide under the GitHub Pages project prefix. It checks code editors, all diagram/source views, source-to-Outline links, compiler success/error feedback, saved drafts, export/import, source/transcript download, chapter switching, deep-link refresh, presenter mode, mobile layout, and the retained Design Patterns and Book I lecture pages. It records fetched response-body sizes and checks that one chapter does not fetch all books.

If needed, install Playwright Chromium with `npx playwright install chromium`, or set `CHROMIUM_PATH` to an installed executable. Browser checks write screenshots under `build/systems-slides-browser/` and a JSON report under `tests/`.

After the offline build, run:

```bash
node tests/systems-slides.offline.mjs
```

Results are recorded in `tests/systems-slides-code-results.json`, `tests/systems-slides-browser-results.json`, and `tests/systems-slides-offline-results.json`. Build sizes and the measured local, uncompressed response-body payload are in `docs/systems-slides-build-report.json`. These payloads are not estimates of GitHub's compressed transfer sizes. Live external compiler availability was not tested.

## Verified delivery results

- **58 automated content/unit tests passed.**
- **305 distinct complete C++ programs** compiled and matched exact stdout, stderr, and exit status.
- Browser tests rendered **all 2,751 pages and 130 diagram/source views** across all 61 chapters, with no page errors.
- The original Book I browser suite passed all **9 test groups**; Design Patterns and Book I lecture data remain unchanged.
- The offline file test passed with **zero network requests**.
- All **1,161 pre-change source/deck hashes** still match; all **2,690 original Systems topic records** are retained exactly.
- Applying the patch to a clean prior project and rebuilding reproduced **all 5,150 generated files byte-for-byte**.
- Desktop, mobile, code, diagram, focused-source, and source-archive layouts were visually inspected. Code and large diagrams scroll within their panels at readable text size.

| Build measure | Before this patch | With Systems lectures |
|---|---:|---:|
| HTML | 1,220 bytes | 1,220 bytes |
| Application JS | 211,101 bytes | 222,362 bytes |
| CSS | 93,478 bytes | 94,777 bytes |
| Master navigation catalog | 42,475 bytes | 42,475 bytes |
| Systems navigation catalog | 793,177 bytes | 793,177 bytes |
| Largest Systems lecture | Not present | 723,544 bytes |
| Total modular dist | 101,862,256 bytes | 122,917,165 bytes |

The measured direct Chapter 1 lecture load was **2,360,670 local uncompressed response-body bytes**. That includes its 479,760-byte original chapter, 723,544-byte lecture, and navigation/interface files. No other book or lecture was fetched. The site keeps the complete library in separate files rather than adding it to initial HTML or JS. The new original narration contains **62,524 words** across **614 teaching pages**, in addition to preserved source explanations.

Live external compiler service availability was not tested. All submitted-code browser checks used the real local compiler through an intercepted request.

## Chapter coverage

| Chapter | Title | Interactive pages | Complete programs | Diagram/source views | Original entries covered |
|---:|---|---:|---:|---:|---:|
| 1 | Understanding Computer Systems: boundaries, artifacts, and evidence | 91 | 5 | 2 | 79/79 |
| 2 | C and C++ for Systems Programming: values, bounds, and lifetime | 76 | 5 | 2 | 63/63 |
| 3 | Bits, Bytes, and Binary Data: operations with explicit meaning | 40 | 5 | 2 | 36/36 |
| 4 | Integer Representation: finite ranges and checked arithmetic | 40 | 5 | 2 | 36/36 |
| 5 | Floating-Point Representation: precision, classification, and numerical contracts | 39 | 5 | 2 | 34/34 |
| 6 | Memory Layout: shape, padding, byte order, and alignment | 41 | 5 | 2 | 38/38 |
| 7 | Pointers and Address Arithmetic: valid ranges and invalidation | 44 | 5 | 2 | 44/44 |
| 8 | Process Memory Layout: storage duration, mappings, and startup input | 44 | 5 | 2 | 44/44 |
| 9 | Dynamic Memory: ownership, growth, and failure cleanup | 45 | 5 | 2 | 46/46 |
| 10 | Memory Allocators: policies, metadata, fragmentation, and lifetime | 48 | 5 | 2 | 52/52 |
| 11 | Memory Errors | 46 | 5 | 2 | 48/48 |
| 12 | Instruction Set Architecture | 42 | 5 | 2 | 40/40 |
| 13 | x86-64 Registers | 49 | 5 | 2 | 54/54 |
| 14 | Assembly Data Movement | 46 | 5 | 2 | 48/48 |
| 15 | Assembly Arithmetic | 45 | 5 | 2 | 46/46 |
| 16 | Machine Control Flow | 45 | 5 | 2 | 46/46 |
| 17 | Procedures and Calling Conventions | 49 | 5 | 2 | 54/54 |
| 18 | Compiler Optimizations | 47 | 5 | 2 | 50/50 |
| 19 | Kernel Fundamentals | 43 | 5 | 2 | 42/42 |
| 20 | Protected Control Transfer | 44 | 5 | 2 | 44/44 |
| 21 | Processes | 44 | 5 | 3 | 42/42 |
| 22 | Virtual Address Spaces | 41 | 5 | 2 | 38/38 |
| 23 | Paging | 42 | 5 | 2 | 40/40 |
| 24 | Page Tables | 43 | 5 | 2 | 42/42 |
| 25 | Translation Hardware | 41 | 5 | 2 | 38/38 |
| 26 | Page Faults | 42 | 5 | 3 | 38/38 |
| 27 | Memory Hierarchy | 47 | 5 | 2 | 50/50 |
| 28 | Locality | 42 | 5 | 2 | 40/40 |
| 29 | Cache Organization | 45 | 5 | 2 | 46/46 |
| 30 | Cache Behavior | 43 | 5 | 2 | 42/42 |
| 31 | Cache Replacement — choosing what to forget | 42 | 5 | 2 | 40/40 |
| 32 | Cache Performance — from access costs to measured work | 43 | 5 | 2 | 42/42 |
| 33 | Files — byte ranges, positions, and object identity | 41 | 5 | 2 | 38/38 |
| 34 | File Descriptors — ownership and exact transfer progress | 45 | 5 | 2 | 46/46 |
| 35 | Buffered I/O — where the bytes are waiting | 43 | 5 | 2 | 42/42 |
| 36 | Memory-Mapped Files — extent, backing, and lifetime | 42 | 5 | 2 | 40/40 |
| 37 | Process Creation — private values and inherited references | 43 | 5 | 2 | 42/42 |
| 38 | Program Execution — replacing an image without losing the contract | 42 | 5 | 2 | 40/40 |
| 39 | Process Termination — decoding results and reaping children | 42 | 5 | 2 | 40/40 |
| 40 | Pipes and Redirection — bytes, EOF, and endpoint ownership | 45 | 5 | 2 | 46/46 |
| 41 | Signals: safe notification and coordinated waiting | 46 | 5 | 2 | 48/48 |
| 42 | Shell Architecture: from typed syntax to jobs | 45 | 5 | 2 | 46/46 |
| 43 | Concurrent Execution: schedules, ownership, and stable results | 43 | 5 | 2 | 42/42 |
| 44 | Threads: identities, completion, and lifetime | 43 | 5 | 2 | 42/42 |
| 45 | Race Conditions: atomic accesses versus correct transactions | 42 | 5 | 2 | 40/40 |
| 46 | Atomic Operations: values, ordering, and publication | 43 | 5 | 3 | 40/40 |
| 47 | Mutual Exclusion: protect invariants and return snapshots | 44 | 5 | 2 | 44/44 |
| 48 | Condition Variables: durable predicates and safe waiting | 44 | 5 | 2 | 44/44 |
| 49 | Producer–Consumer Systems: capacity, ownership, and closure | 46 | 5 | 4 | 44/44 |
| 50 | Deadlock: dependency graphs and coordinated acquisition | 45 | 5 | 2 | 46/46 |
| 51 | Network Programming | 44 | 5 | 2 | 44/44 |
| 52 | Concurrent Servers | 41 | 5 | 2 | 38/38 |
| 53 | Event-Driven I/O | 44 | 5 | 2 | 44/44 |
| 54 | Measuring Performance | 43 | 5 | 2 | 42/42 |
| 55 | Program Optimization | 44 | 5 | 2 | 44/44 |
| 56 | Performance Analysis | 43 | 5 | 2 | 42/42 |
| 57 | Memory-Safety Vulnerabilities | 41 | 5 | 2 | 38/38 |
| 58 | Exploit Mitigations | 41 | 5 | 2 | 38/38 |
| 59 | System Robustness | 44 | 5 | 2 | 44/44 |
| 60 | From Source Code to Running Program | 48 | 5 | 2 | 52/52 |
| 61 | Putting the System Together | 55 | 5 | 5 | 52/52 |
| **Total** | **61 chapters** | **2751** | **305** | **130** | **2690/2690** |

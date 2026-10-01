# Design Patterns: interactive lectures for every chapter

This patch extends the interactive Slides page supplied for C++ Book I, Chapter 1 to all **24 Design Patterns chapters, 0–23**. The author credit is **Dr. Charles Dorner**. The original Book I lecture remains at **26 slides**, with its original lecture content unchanged.

The Design Patterns lectures contain **1,152 slides**, **304 runnable program views**, and **99 diagram/source-focus views**. These are lectures inside Programming Studio, with the same slide navigation, code workbench, presenter notes, and saved editing workflow as the supplied Book I page.

## What each lecture contains

Every chapter contains substantive spoken narration, C++ examples, questions with revealable explanations, teaching directions, and discussion of design choices. The foundation lecture teaches the vocabulary, C++ mechanisms, diagram conventions, and study method used by the rest of the course. Each pattern chapter develops its concrete problem, a simple starting design, the pressure that motivates a change, the mechanism, invariants, failure cases, competing designs, and maintenance consequences.

Chapters 1–23 also bring the actual chapter diagrams and focused source blocks into Slides. UML views use the existing diagram assets and their arrow explanations; focused implementation views include exact source references, line-by-line explanations, and adjacent theory panels. Class 0 adds a class/ownership diagram with actual C++ excerpts from its replaceable-estimate program. Its diagram source is editable Graphviz, and the theory panel explains the arrows and lifetime contract.

The complete chapter programs, extension starters, and extension solutions remain available, alongside all eight existing trace experiments for each chapter from 1 through 23. Progressive chapter exercises include hints and explained answers. The capstone adds a longer integrated design walkthrough.

The count of **304 runnable program views** comprises:

- **51 newly authored small demonstrations**, including baseline designs and controlled failures.
- **69 existing original programs**: 23 chapter programs, 23 extension starters, and 23 extension solutions.
- **184 trace programs**, using the existing eight verified trace cases for each chapter from 1 through 23.

These counts describe complete programs available in Slides. They do not mean that 304 new Coding Lab questions were added. Of the **99 diagram/source-focus views**, 98 reuse the existing diagrams and source blocks; one new Class 0 diagram explains the introductory program. These are not 99 newly created UML assets.

## Chapter coverage

The table is taken from `docs/design-patterns-slides-counts.json`. A reading entry is covered when its existing topic ID is represented in the lecture. The 538 entries belong to chapters 0–23; the separate glossary is retained in the study application.

| Chapter | Lecture | Slides | Runnable program views | Diagram/source-focus views | Reading entries covered |
|---:|---|---:|---:|---:|---:|
| 0 | Foundations: read, test, and choose a design pattern | 51 | 4 | 1 | 13/13 |
| 1 | Factory Method: vary construction without copying the job | 46 | 13 | 4 | 22/22 |
| 2 | Abstract Factory: create products that agree | 46 | 13 | 4 | 22/22 |
| 3 | Builder: cross a checked boundary from draft to product | 46 | 13 | 4 | 22/22 |
| 4 | Prototype: copy the actual object and define what stays independent | 46 | 13 | 4 | 22/22 |
| 5 | Singleton: separate one instance from safe shared use | 46 | 13 | 4 | 22/22 |
| 6 | Adapter | 47 | 13 | 4 | 22/22 |
| 7 | Bridge | 47 | 13 | 4 | 22/22 |
| 8 | Composite | 47 | 13 | 4 | 22/22 |
| 9 | Decorator | 47 | 13 | 4 | 22/22 |
| 10 | Facade | 47 | 13 | 4 | 22/22 |
| 11 | Flyweight | 48 | 13 | 4 | 23/23 |
| 12 | Proxy | 48 | 13 | 4 | 23/23 |
| 13 | Chain of Responsibility | 46 | 13 | 4 | 22/22 |
| 14 | Command — requests, history, and honest undo | 46 | 13 | 4 | 22/22 |
| 15 | Iterator — positions, boundaries, and valid traversal | 47 | 13 | 4 | 23/23 |
| 16 | Mediator — one home for a collaboration rule | 47 | 13 | 4 | 23/23 |
| 17 | Memento — saved values, private state, and restoration | 47 | 13 | 4 | 23/23 |
| 18 | Observer — registration, lifetime, and delivery contracts | 47 | 13 | 4 | 23/23 |
| 19 | State | 47 | 13 | 4 | 23/23 |
| 20 | Strategy | 46 | 13 | 4 | 22/22 |
| 21 | Template Method | 46 | 13 | 4 | 22/22 |
| 22 | Visitor | 47 | 13 | 4 | 23/23 |
| 23 | Capstone: one monitor, several interacting guarantees | 74 | 14 | 10 | 33/33 |
| **Total** | **24 chapter lectures** | **1152** | **304** | **99** | **538/538** |

## Prerequisites

Apply this patch to the project that already has:

1. The supplied **Book_I_Chapter_1_Slides.patch** installed. Its downloaded filename may include a suffix such as `(1)`; it is the same required Book I Slides feature.
2. The previous **Design Patterns Study Expansion** installed. Its Design Patterns interface shows **1,009 flashcards, 47 UML overview views, and 50 Coding Lab questions**.

The Outline Examples follow-up patch is **not required**. If it is already installed, its separate Outline teaching additions remain in place. The lecture generator reads the existing chapter sources and trace cards directly.

Use Node.js **20.11 or newer**, npm, and the project dependencies declared in `package.json` and `package-lock.json`. Local C++ verification requires a C++20-capable compiler such as the documented GCC toolchain. Browser tests use the project's Playwright dependency and a Chromium installation.

## Apply the changed-code patch

Download `Design_Patterns_All_Chapters_Slides.patch` to your Downloads folder, then run:

```bash
cd "/home/charles/Documents/Design_Patterns_Study_Studio_Source (2)/Design_Patterns_Study_Studio" &&
git apply --check "$HOME/Downloads/Design_Patterns_All_Chapters_Slides.patch" &&
git apply "$HOME/Downloads/Design_Patterns_All_Chapters_Slides.patch" &&
npm run build &&
npm test &&
npm start
```

The `&&` operators stop the sequence if a command fails. `git apply` works in this extracted project folder even when that folder is not a Git repository. No `git init` is required. If the check reports a mismatch, keep the error and reconcile the changed files before applying; do not force an overwrite of unrelated local work.

If this is a fresh dependency installation, run `npm ci` in the project folder before the build. `npm start` serves the generated `dist/` at `http://localhost:5173/` and stays running until Ctrl+C.

After checking the result, use your normal rsync workflow to copy the updated project into your actual Git working tree, then inspect and commit the changes there. The patch does not deploy to GitHub or modify the hosted site by itself. The existing GitHub Pages deployment should continue to publish `./dist`.

## Find the lectures

Open **Design Patterns → choose a chapter → Slides**. The Slides chapter menu includes Class 0, all 22 pattern chapters, and the capstone. The slide menu identifies the teaching section for each slide. Previous/Next and arrow navigation move through the lecture; editing inside a code field does not turn those typing keys into slide navigation.

Useful local links after `npm start`:

- Class 0: `http://localhost:5173/?course=design-patterns-cpp&chapter=0&view=slides&slide=1`
- Factory Method: `http://localhost:5173/?course=design-patterns-cpp&chapter=1&view=slides&slide=1`
- Observer: `http://localhost:5173/?course=design-patterns-cpp&chapter=18&view=slides&slide=1`
- Capstone: `http://localhost:5173/?course=design-patterns-cpp&chapter=23&view=slides&slide=1`
- Original Book I page: `http://localhost:5173/?course=cpp-book-01&chapter=1&view=slides&slide=1`

The `slide` query parameter is one-based. A refresh restores the requested lecture and slide. The deployed form uses the project prefix, for example:

```text
https://dornercr.github.io/Programming_Studio_Studio/?course=design-patterns-cpp&chapter=19&view=slides&slide=1
```

To preview that prefix locally, stop the ordinary development server and run:

```bash
cd "/home/charles/Documents/Design_Patterns_Study_Studio_Source (2)/Design_Patterns_Study_Studio" &&
BASE_PATH=/Programming_Studio_Studio/ npm start
```

Then open `http://localhost:5173/Programming_Studio_Studio/?course=design-patterns-cpp&chapter=1&view=slides&slide=1`.

## Use code, diagrams, and presenter notes

Predict a result before running the complete example. The expected-output panel is labeled as the original chapter expectation; it is not represented as a live result of edited code. Actual run results show build diagnostics, execution status, stdout, and stderr. If an edit changes the code after a run, the output panel marks that result as stale.

Code can be edited, reset, restored with Undo replace, and downloaded as a `.cpp` file. Each complete example includes a local build command. C++ excerpts are explicitly labeled as excerpts and are not offered as standalone programs. The language selector supports C++20 and C++17; the delivered Design Patterns programs and verification use C++20.

The diagram button enlarges a diagram for inspection. Focused source views expose line-by-line explanations beside the corresponding contract. The pane divider lets a presenter give more space to the slide or the code. On smaller screens, the layout adapts instead of shrinking source text into an unreadable slide image.

Open **Presenter script and directions** for the verbatim SAY text, DO directions, and source reference. Presenter mode closes these notes on entry. **Download transcript** exports the current chapter's full script and directions as text. Generated transcript files are also included under `lectures/transcripts/design-patterns-cpp/`.

## Local study state and code submission

Lecture position, pane ratio, code drafts, input, and selected language are saved separately for each course and chapter under the existing Slides storage key:

```text
patterns-study-studio:slides:v1
```

Drafts also have a slide ID, so editing one demonstration does not replace another chapter's code. The existing reading progress, flashcard state, notes, bookmarks, and Coding Lab state keep their original storage and IDs. This patch does not require a migration of those records.

Use the Slides **Export lecture work** control to back up lecture drafts and position. Import that file while the matching course and chapter are open. The importer checks the backup's version, course, and chapter and accepts drafts only for known slides. The application's general study backup remains available for its existing study records; **lecture work has its own export/import and must be backed up separately**.

Reading, editing, expected-output inspection, transcript download, and local source download do not submit code. **Run online** explicitly sends the current editor source and stdin to Compiler Explorer at the configured external endpoint:

```text
https://godbolt.org/api/compiler/g142/compile
```

Run online requires internet and that service to be available. It is not an in-browser compiler or a local shell. Stop waiting cancels the browser's wait; a request already submitted may still finish remotely. Keep a local `.cpp` copy if you want to compile with your own toolchain.

The remote service's live availability is **not verified by local test success**. The supplied C++ verification script uses a real local compiler. The browser test intercepts compiler requests and compiles them locally, so browser QA does not submit source to the public service.

## Authoritative source and rebuild architecture

The existing educational content remains authoritative. The new lecture layer reads it without rewriting the chapter text, original examples, original solutions, cards, labs, diagrams, or study IDs.

| Location | Role |
|---|---|
| `lectures/authoring/design-patterns/ch0.json` through `ch23.json` | Editable original lecture teaching, spoken narration, questions, and small demonstrations. |
| `manuscript/ch01.json` through `ch23.json` | Existing chapter sections, requirements, theory, traces, exercises, and lab instructions. |
| `content/design-patterns-cpp/ch0.json` through `ch23.json` | Existing reading entries, diagram selectors, code bundles, and trace cards. |
| `companion/examples/`, `companion/exercises/`, `companion/solutions/` | Original C++ source used by complete-program and source-reference views. |
| `scripts/build-design-patterns-slides.mjs` | Generates every Design Patterns lecture shard, transcript, downloadable C++ file, counts, and source-integrity manifest. |
| `lectures/design-patterns-cpp/chN.json` | Generated per-chapter lecture shards loaded by the web application. |
| `lectures/examples/design-patterns-cpp/chN/` | Generated standalone C++ downloads for runnable lecture views. |
| `lectures/transcripts/design-patterns-cpp/chN.txt` | Generated per-chapter spoken scripts and directions. |
| `lectures/diagrams/design-patterns-ch0.dot` | Editable source for the introductory diagram; regenerated as SVG during a normal build. |
| `lectures/manifest.json` | Small catalog of available chapter lectures; retains Book I's entry. |
| `lectures/design-patterns-source-integrity.json` | SHA-256 manifest for the existing source files used to construct the lecture layer. |
| `src/slides.js`, `src/slides.css` | Shared interactive lecture renderer, editing and presentation controls, and layout. |
| `scripts/build-slides.mjs` | Runs lecture generation and the existing site build, then attaches the renderer and copies generated resources. |

Edit the authoring JSON or the actual authoritative chapter source as appropriate, then rebuild. Do not maintain independent hand-edited versions of generated lecture shards or `dist/`.

```bash
npm run build &&
npm test
```

A normal build regenerates the lecture data deterministically from its inputs. Source blocks retain their exact files and line references, and complete-program downloads are emitted from the same source text used in the lecture editor. Tests compare those representations rather than assuming they agree.

The web build remains modular. Its application JavaScript contains the renderer and lightweight lecture catalog, not the full lecture library. Opening the ordinary Outline does not require a lecture fetch. Opening Slides fetches only the selected chapter's lecture JSON through the existing relative-path loader. Previously opened decks are cached in memory. Other books and chapter lectures are not preloaded as a side effect. Asset and data URLs stay relative to the GitHub Pages project path.

## Optional offline distribution

Generate the separate offline distribution with:

```bash
npm run build:offline
```

This creates the existing portable build at `dist-offline/index.html` with lecture data included for offline reading and editing. Keep the generated offline directory and its accompanying resources together. The normal Pages build still publishes the modular `dist/`; the offline bundle is a separate choice and does not replace that architecture.

Offline lecture editing and saved drafts work where browser storage is available. A self-contained offline lecture does not make Compiler Explorer available offline. Use the displayed local build commands and a local C++ compiler to run downloaded code without internet.

## Reproduce the verification

Before packaging, 53 automated tests passed; every one of the 304 runnable program views matched the printed output, stderr, and exit status (289 distinct compilations). Browser tests rendered all 1,152 slides and 99 diagram/source views, checked mobile layout and lecture-work import/export, and used a real local compiler for success and error feedback. The original Book I browser suite passed all nine groups. The offline file:// test loaded diagrams, switched chapters, edited code, and restored drafts without any network requests. The live external compiler service was not tested.

The commands below reproduce these checks. Read their actual exit status and generated results.

Build and run all unit/content-integrity tests:

```bash
npm run build &&
npm test
```

The dedicated lecture tests check all 24 chapter entries, narration and runnable-code coverage, every represented reading topic, source hashes, diagram selectors, assets, focused source line references, downloadable code equality, lazy-loading structure, and the retained 26-slide Book I page.

Compile and execute every complete Design Patterns lecture program using the local compiler:

```bash
node scripts/test-design-patterns-slides-code.mjs
```

By default this uses `g++`, C++20, warnings, and four concurrent compiler jobs. To choose another installed compatible compiler or reduce concurrency:

```bash
CXX=g++ CPP_JOBS=2 node scripts/test-design-patterns-slides-code.mjs
```

The script compares stdout, stderr, exit status, and expected build/run phase. It writes `tests/design-patterns-slides-code-results.json`. Intentionally broken build examples must fail at compilation; they are not counted as successful runtime demonstrations merely because a compiler emitted text.

Run the full Design Patterns browser suite after building:

```bash
node tests/design-patterns-slides.browser.mjs
```

The suite starts its own local server beneath `/Programming_Studio_Studio/`, visits every slide, resolves the linked diagram/source views, checks chapter navigation and deep links, exercises code editing and local compiler feedback, checks download and lecture-work restore, verifies mobile width, and reopens the original Book I lecture. It records the initial requests and response-body byte counts to detect accidental library-wide loading. It writes `tests/design-patterns-slides-browser-results.json` and screenshots under `build/design-patterns-slides-browser/`.

If Playwright cannot find Chromium, install its browser with `npx playwright install chromium`, or set `CHROMIUM_PATH` to the absolute path of an installed Chromium executable:

```bash
CHROMIUM_PATH=/absolute/path/to/chromium node tests/design-patterns-slides.browser.mjs
```

Run the original supplied Slides browser regression suite as well:

```bash
npm run test:slides-browser
```

Inspect the generated desktop and mobile screenshots at normal size. Browser geometry checks catch overflow, but visual review is also needed for text density, spacing, code legibility, and the relationship between a source block and its theory panel. Rebuild and retest after an authoring or renderer change.

## Measured web payload

The verified direct Chapter 1 Slides request fetched **784,206 response-body bytes** from the local uncompressed server (about 766 KiB), including its requested chapter and lecture. It did not fetch any other book or lecture. This is not a claim about GitHub's compressed transfer size. The measured components were:

| Component | Bytes |
|---|---:|
| HTML | 1,220 |
| Application JavaScript | 211,101 |
| Loader JavaScript | 3,355 |
| CSS | 93,478 |
| Master catalog | 42,475 |
| Design Patterns navigation catalog | 216,233 |
| Requested source chapter | 78,690 |
| Requested lecture | 137,654 |

The ordinary Outline request loads no lecture JSON. Diagram assets load when their slides are opened. The full source library remains in separate resources.

To verify the optional offline build after generating it:

```bash
node tests/design-patterns-slides.offline.mjs
```

# Study Studio — Design Patterns and Systems Programming

An offline study companion with two separate courses credited to **Dr. Charles Dorner**: Design Patterns in C++ and Systems Programming and Machine Organization. It uses the supplied Study Studio design, chapter navigation, notes, bookmarks, progress, and practice tools.

## Start studying

Open `dist/index.html` in a modern desktop browser. You can copy that one file anywhere; the content, CSS, JavaScript, and SVG diagrams are embedded. No account, web server, internet connection, or installation is needed to study. A separately supplied `Design_Patterns_Study_Studio.html` is the same app.

Select **Design Patterns in C++** in the curriculum dropdown. Choose a chapter in the chapter selector or sidebar. Switch between five views:

1. **Outline:** a numbered chapter map with named sections and linked lessons. Every chapter opens with a concrete problem and a worked example. Follow code, expected output, an execution trace, a common mistake, alternatives, and a discussion with an explained answer. The sidebar follows the same chapter → section → lesson hierarchy.
2. **Flashcards:** try to recall the answer, flip the card, then mark it Known or Again. The review queue collects cards marked Again; this is a manual review queue, not a time-scheduled repetition algorithm.
3. **Scenarios:** choose among four designs or diagnoses, then read why each option is right or wrong. Retry or move to another case.
4. **UML:** inspect the overview, open a larger version, and study each focused C++ block with its adjacent theory panel and expandable line-by-line explanation.
5. **C++:** switch among the chapter example, lab starter, and lab solution. Read the expected output, download the source, and build it locally.

The chapter selector stays available in every view. **All chapters** makes the chapter navigation unrestricted; UML and C++ show the chapter of the currently selected lesson. Foundations and the glossary link you to a chapter with a worked program. The glossary is searchable. The sidebar switch groups topics by chapter or by subject area (pattern family in Design Patterns).

A useful study session: read the problem, explain why the first design fails, recall the pattern without looking, trace its UML to the code, answer a scenario, then modify the lab. Use **My study material** to add personal study topics.

## Systems Programming course — teaching revision 1.3

Choose **Systems Programming and Machine Organization** from the curriculum dropdown. Every one of its **61 chapters** now uses the same five-part learning route as Design Patterns: start with the problem, understand the mechanism, trace code and behavior, discuss and practice, then connect and review.

The Systems course includes:

| Material | Count |
| --- | ---: |
| Original slide lessons, preserved with transcripts | 1,775 |
| Original application cases | 61 |
| New authored learning and exercise entries | 854 |
| Independent progressive exercises with hints and explained answers | 366 |
| Recall cards, including contextual vocabulary | 1,049 |
| Glossary definitions with chapter links | 122 |
| UML / activity / state / ownership / sequence views | 66 |
| Focused code blocks with line-by-line explanations and adjacent theory | 64 |
| Complete chapter demonstrations | 61 |
| Complete lab starters and reference solutions | 122 |

There are 2,691 study entries including the glossary. Paired Deep Dive slides remain attached to their main source lesson, giving 2,007 entries in the default reading order. All original study IDs remain stable so saved notes, bookmarks, and progress still have their original targets. The approved Design Patterns curriculum is unchanged; its canonical digest is checked in the test suite.

Systems now offers **all six views**: Outline, Flashcards, Scenarios, UML, C++, and Lectures. Its C++ tab has Example, Lab starter, and Lab solution choices. Each shows the exact packaged filename, complete source, build command, expected output, and verification or acceptance criteria. Downloaded files receive the short filename shown in the browser build command; package files keep their documented paths.

Begin with the chapter problem and its small demonstration. Read the selected code line by line and explain each state transition. Then attempt the six independent exercises: guided modification, tracing, bug diagnosis, design choice, extension, and justification. Hints and explained answers are separate disclosures. Each exercise names the required source material; none depends on another exercise’s edits. The full lab expands the chapter model into a reusable, checked operation.

The capstone solution integrates a bounded work queue, two real workers, owned messages, input rejection, stable result ordering, and close-and-collect shutdown. Its deterministic output checks each indexed result as well as the aggregate totals and an empty-input run. Additional focused views explain queue waiting, admission, and exceptional cleanup.

**Models and real operations.** Hardware and OS demonstrations are explicitly labeled C++ models. They do not implement a kernel, perform a real fork/exec/mmap, create a network server, or measure a processor cache. The thread, atomic, mutex, condition-variable, and concurrent queue labs do execute standard C++ synchronization. Their deterministic fixtures test stated contracts; they do not prove all possible schedules or provide performance benchmarks. The reference programs assume the bounded input ranges stated in each lab. Byte-sum fixtures check the required character values at compile time; the floating-point demonstration checks binary64 and assumes the ordinary round-to-nearest execution environment.

**Original lectures.** The Lectures tab retains each supplied PowerPoint and complete matching transcript byte for byte, including the original slides, notes, diagrams, and equations. The study reader does not recreate a PowerPoint canvas. Original companion listings named by some source lectures were absent from the supplied files and are not claimed as tested; the added examples are independently authored programs. The visible correction to the source race-condition story remains: unsynchronized ordinary C++ increments have undefined behavior, while the single-threaded and atomic models demonstrate defined lost updates.

## Edit and verify the Systems material

- `scripts/teaching_examples.py`: chapter demonstrations, original traces, alternatives, and discussions.
- `scripts/systems_labs.py`: lab requirements, complete reference programs, checks, failure explanations, and vocabulary.
- `scripts/systems_focus.py`: exact code selections and authored line-by-line explanations.
- `scripts/systems_relationships.py`: additional state, ownership, class, and sequence views plus expanded queue/capstone blocks.
- `scripts/build_systems_depth.py`: the five-part learning sequence, exercises, and content assembly.
- `teaching/systems-depth.json` and `teaching/systems-labs.json`: editable generated content for inspection; regenerate from the scripts to preserve changes across builds.
- `systems_examples/chNN/`: complete demonstrations and exact output fixtures.
- `systems_labs/chNN/`: starter, solution, separate expected outputs, and lab README.
- `systems_source/`: unchanged original chapter presentations and transcripts.

Compile with a C++17 compiler and standard threading support. The validated platform uses GCC 13.3 on Linux. Use the documented commands with assertions enabled; do not add `-DNDEBUG` because assertions contain verification fixtures. There are no third-party C++ dependencies. GNU-style compiler flags are used below; other toolchains need equivalent flags.

```sh
python3 scripts/test_workshops.py
python3 scripts/test_systems_labs.py
```

The first command checks all 61 Systems demonstrations plus Design Patterns foundations. The second checks all 122 Systems lab files. Both build independent programs with `-std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread`, execute them with timeouts, and compare stdout byte for byte. Lab results also record source hashes. Tests cover ordinary, boundary, and rejection cases named in each lab; their scope is not a general proof of correctness.

After installing Playwright and Chromium, run:

```sh
node scripts/systems_depth_browser_test.mjs
node scripts/systems_browser_test.mjs
node scripts/teaching_browser_test.mjs
node scripts/check_reading_text.mjs
```

These check the new Systems views, all original lecture downloads and slide records, chapter navigation, independent notes, backup compatibility, source text, and mobile overflow. Representative screenshots are in `docs/preview-systems-depth-*.png`. `docs/SYSTEMS_REVISION.md` and the JSON verification reports record the delivered checks.

## Design Patterns coverage

| Content | Count |
| --- | ---: |
| Chapter groups | 24: foundations, 22 patterns, capstone |
| Lessons, including exercises and glossary | 539 |
| Flashcards | 609 |
| Original four-option application cases | 49 |
| Glossary entries | 92 |
| Diagram views | 25 |
| Focused code/theory blocks | 51 |
| Complete C++ programs | 70: 23 examples, 23 starters, 23 solutions, 1 foundations program |

The companion retains the named sections of the editable textbook manuscript, worked-example contracts and alternatives, execution traces, failure cases, focused explanations, chapter exercises and answers, labs, and summaries. The original manuscript JSON is included for editing and traceability. It is a study companion to the textbook, not a PDF viewer or a replacement for the textbook’s page design. Biography and print-only front matter are not turned into study questions. `docs/coverage.json` records the generated counts.

## UML and source references

Every worked-example chapter has an overview diagram. The capstone also has state and sequence views. The UML tab includes a text form of the relationships for each overview. Each focused block includes:

- A numbered excerpt from the exact packaged `main.cpp`.
- A theory panel explaining what the block means, where it fits, why it is here, what must stay true, and what can go wrong.
- The exact source filename and line range.
- An expandable explanation of every displayed source line.

Read each view’s legend. Hollow triangle arrows point from a derived type toward its base type. A filled diamond marks the owner end of a composition relationship. Dashed arrows indicate dependencies; green dashed arrows are labeled shared relationships. Activity and state arrows represent execution or allowed transitions, as labeled. Sequence arrows are calls and time runs downward. Teaching panels are annotations, not additional UML relationships. Overview snippets can omit surrounding code; the C++ view, `companion/**/main.cpp`, `systems_examples/chNN/main.cpp`, and the Systems lab `.cpp` files are complete programs.

On a phone, diagrams and long source lines scroll within their own panels. The focused theory panel follows its code block. A desktop screen provides the side-by-side layout.

## Notes, progress, and backup

Notes and progress are saved in this browser’s local storage under `patterns-study-studio:v1`. They are not sent anywhere. They are separate from the original supplied app’s saved data. Browser privacy settings, private browsing, clearing site data, or moving the HTML to a new location may change whether saved data is available. Use **Settings → Export backup** to keep a portable copy. **Import / restore** asks before replacing saved progress. A **study pack** exports curriculum content without your private notes and progress.

For consistent local storage across rebuilds, you can use the optional localhost server below. Keep the same port. The server binds only to `127.0.0.1`.

## Rebuild the standalone app

Requirements: **Node.js 20 or newer**. The basic rebuild uses only Node’s built-in modules and the included compiled Tailwind CSS. No dependency install is required.

From this directory:

```sh
node scripts/build.mjs
node --test tests/*.test.mjs
```

Result: `dist/index.html`.

Optional localhost preview:

```sh
node scripts/serve.mjs
```

Open `http://localhost:5173`. Stop the server with Ctrl+C.

## Regenerate the study content and diagrams

Requirements: **Python 3.10 or newer**, Node.js 20+, and the pinned development dependency `@viz-js/viz` 3.25.0. Python uses only its standard library. Dependency installation requires network access; studying and the basic rebuild do not.

```sh
npm ci
python3 scripts/make_content.py
node scripts/render_diagrams.mjs
node scripts/render_systems_diagrams.mjs
node scripts/build.mjs
npm test
```

Run these commands in this order. Content generation clears the embedded diagram images; the diagram renderer restores them. The renderer also writes SVG assets and DOT sources. Class, activity, ownership, and state views use Graphviz through Viz.js. The sequence view uses the exact positioned SVG renderer in `render_diagrams.mjs`; its JSON plus that script is the canonical editable source.

Edit chapter prose and exercises in `manuscript/`, program files and expected output in `companion/`, and overview relationships plus focused explanations in `diagrams/chNN.json`. Edit scenario choices and rationales in `scripts/scenarios.py`. If source lines move, update `resolved_start` and `resolved_end` in the matching diagram JSON, plus the first/last marker text and line explanations. The generator and tests reject stale excerpts.

Edit the application in `src/app.js`, `src/teaching.js`, and `src/styles.css`, then run the basic rebuild. `src/content.json` is generated but may also be edited directly for a one-off study pack. Regeneration replaces direct JSON edits.

The included `vendor/tailwind.css` is compiled from Tailwind CSS 4.1.10 and is sufficient for every class currently used. Use `src/styles.css` for additional styles. `src/theme.css` preserves the original Tailwind theme source; the basic build does not rerun Tailwind. Its MIT license is in `docs/licenses/`.

## Build and verify the C++ programs

Programs use **C++17 and the C++ standard library only**. A compiler with complete C++17 support is required. The supplied test harness uses GCC/Clang-style flags. This release was tested with GCC 13.3.0 on Linux. No extra C++ libraries are needed. Each `main.cpp` is independent; do not link all chapters together.

From the project root, build one chapter:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread companion/examples/ch01/main.cpp -o chapter01
./chapter01
```

Compare the output with `companion/examples/ch01/expected.txt`. Chapter-specific lab instructions and explained solutions are in the app and editable manuscript.

Run all 69 programs and compare their actual standard output byte-for-byte with the expected files:

```sh
python3 companion/tests/run_all.py --jobs 4
```

Run only a chapter or substitute Clang:

```sh
python3 companion/tests/run_all.py --chapter 01
CXX=clang++ python3 companion/tests/run_all.py --jobs 4
```

Optional undefined-behavior sanitizer pass:

```sh
python3 companion/tests/run_all.py --sanitize --jobs 4
```

This optional sanitizer command is provided for further checks; the included `verification.json` records the ordinary GCC run. Compiled binaries go under `companion/build/` and can be removed. Do not use `-DNDEBUG` when relying on the examples’ assertion checks.

## Browser checks and preview images

The content test uses Node’s built-in test runner. It checks chapter coverage, practice IDs, four-option scenarios, exact code excerpts and line references, and equality of embedded C++/outputs with the companion files.

Optional browser checks require the pinned Playwright development dependency (1.62.1) and its Chromium browser:

```sh
npm ci
npx playwright install chromium
node scripts/browser_test.mjs
node scripts/visual_check.mjs
```

The browser test checks offline loading, every chapter in each applicable view, local persistence, card review, scenario feedback and retry, large diagrams, C++ download, glossary search, mobile navigation and width, backup/restore, and browser errors. Results are in `tests/browser-results.json`. Preview PNGs are in `docs/`.

## Files

- `dist/index.html`: the complete offline app.
- `src/`: editable app, styles, HTML shell, generated study pack, and theme source.
- `manuscript/`: editable textbook content used to create the companion.
- `diagrams/`: overview definitions, focused source references, and generated DOT sources.
- `assets/`: all 25 generated SVG diagram views.
- `companion/`: complete C++ examples, exercises, solutions, expected output, tests, and verification report.
- `scripts/`: reproducible content, diagram, app-build, server, and browser-check scripts.
- `tests/`: automated content tests for both courses and browser results.
- `systems_source/`: all supplied original PowerPoints, transcripts, and the lecture-series coverage map.
- `vendor/`: compiled CSS for an offline, dependency-free basic rebuild.
- `docs/`: coverage, validation notes, previews, and third-party license.

No service worker, CDN, remote font, analytics, or network API is needed by the delivered app. Programs are displayed and downloaded; the browser does not compile C++.

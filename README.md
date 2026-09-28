# Study Studio — Design Patterns and Systems Programming

An offline study companion with two separate courses credited to **Dr. Charles Dorner**: Design Patterns in C++ and Systems Programming and Machine Organization. It uses the supplied Study Studio design, chapter navigation, notes, bookmarks, progress, and practice tools.

## Start studying

Open `dist/index.html` in a modern desktop browser. You can copy that one file anywhere; the content, CSS, JavaScript, and SVG diagrams are embedded. No account, web server, internet connection, or installation is needed to study. A separately supplied `Design_Patterns_Study_Studio.html` is the same app.

Select **Design Patterns in C++** in the curriculum dropdown. Choose a chapter in the chapter selector or sidebar. Switch between five views:

1. **Outline:** explanations, requirements, alternatives, traces, failure cases, exercises, hints, and explained answers.
2. **Flashcards:** try to recall the answer, flip the card, then mark it Known or Again. The review queue collects cards marked Again; this is a manual review queue, not a time-scheduled repetition algorithm.
3. **Scenarios:** choose among four designs or diagnoses, then read why each option is right or wrong. Retry or move to another case.
4. **UML:** inspect the overview, open a larger version, and study each focused C++ block with its adjacent theory panel and expandable line-by-line explanation.
5. **C++:** switch among the chapter example, lab starter, and lab solution. Read the expected output, download the source, and build it locally.

The chapter selector stays available in every view. **All chapters** makes the chapter navigation unrestricted; UML and C++ show the chapter of the currently selected lesson. Foundations and the glossary link you to a chapter with a worked program. The glossary is searchable. The sidebar switch groups topics by chapter or by subject area (pattern family in Design Patterns).

A useful study session: read the problem, explain why the first design fails, recall the pattern without looking, trace its UML to the code, answer a scenario, then modify the lab. Use **My study material** to add personal study topics.

## Systems Programming course

Choose **Systems Programming and Machine Organization** from the curriculum dropdown. It contains all **61 chapters**, **1,775 slide lessons**, **805 recall cards**, and **61 new application cases**. Including the case lessons, its outline contains **1,836 lessons**. Each slide lesson preserves the student slide’s text and offers an expandable, matching professor transcript section. Chapter and subject-area navigation, search, bookmarks, personal notes, review marks, and practice progress work independently for this course.

The **Lectures** tab lets you jump to a slide lesson, read a complete chapter transcript, or download its original PowerPoint and transcript. PowerPoint downloads retain the supplied diagrams, images, equations, and notes. The app presents extracted slide text; it does not claim that a text extraction reproduces the visual slide canvas. Open a downloaded PowerPoint for the original visual layout.

Lecture programs are teaching excerpts unless the supplied source identifies them as complete. No separate compilable systems-programming source package was supplied in the lecture ZIP, and this addition does not claim to compile its snippets. The verified C++ examples in the Design Patterns course remain available through its C++ tab.

The full supplied lecture files are in `systems_source/`. The extraction and coverage script is `scripts/make_systems.py`; the newly authored application cases are in `scripts/systems_scenarios.py`. `docs/systems-coverage.json` maps every chapter and records the hashes of its source files. The normal `make_content.py` regeneration includes both courses. The combined standalone HTML is about 20 MB because it embeds all 61 original presentations as well as the text.

Study modes adapt to the selected course: Design Patterns offers UML and C++, and Systems Programming offers Lectures. Notes use the same local storage key as version 1.0. Existing Design Patterns topic IDs and backup compatibility are preserved. Export a backup before moving or renaming a locally opened HTML file, since browsers can associate local-file storage with its location.

To run the added browser checks after installing Playwright and Chromium:

```sh
node scripts/systems_browser_test.mjs
```

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
| Complete C++ programs | 69: 23 examples, 23 starters, 23 solutions |

The companion retains the named sections of the editable textbook manuscript, worked-example contracts and alternatives, execution traces, failure cases, focused explanations, chapter exercises and answers, labs, and summaries. The original manuscript JSON is included for editing and traceability. It is a study companion to the textbook, not a PDF viewer or a replacement for the textbook’s page design. Biography and print-only front matter are not turned into study questions. `docs/coverage.json` records the generated counts.

## UML and source references

Every worked-example chapter has an overview diagram. The capstone also has state and sequence views. The UML tab includes a text form of the relationships for each overview. Each focused block includes:

- A numbered excerpt from the exact packaged `main.cpp`.
- A theory panel explaining what the block means, where it fits, why it is here, what must stay true, and what can go wrong.
- The exact source filename and line range.
- An expandable explanation of every displayed source line.

Read each view’s legend. Hollow triangle arrows point from a derived type toward its base type. A filled diamond marks the owner end of a composition relationship. Dashed arrows indicate dependencies; green dashed arrows are labeled shared relationships. Activity and state arrows represent execution or allowed transitions, as labeled. Sequence arrows are calls and time runs downward. Teaching panels are annotations, not additional UML relationships. Overview snippets can omit surrounding code; only the C++ view and `companion/**/main.cpp` files are complete programs.

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
node scripts/build.mjs
npm test
```

Run these commands in this order. Content generation clears the embedded diagram images; the diagram renderer restores them. The renderer also writes SVG assets and DOT sources. Class, activity, ownership, and state views use Graphviz through Viz.js. The sequence view uses the exact positioned SVG renderer in `render_diagrams.mjs`; its JSON plus that script is the canonical editable source.

Edit chapter prose and exercises in `manuscript/`, program files and expected output in `companion/`, and overview relationships plus focused explanations in `diagrams/chNN.json`. Edit scenario choices and rationales in `scripts/scenarios.py`. If source lines move, update `resolved_start` and `resolved_end` in the matching diagram JSON, plus the first/last marker text and line explanations. The generator and tests reject stale excerpts.

Edit the application in `src/app.js` and `src/styles.css`, then run the basic rebuild. `src/content.json` is generated but may also be edited directly for a one-off study pack. Regeneration replaces direct JSON edits.

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

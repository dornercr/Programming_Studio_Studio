# Systems Programming — parity revision 1.3

The Systems course now follows the approved Design Patterns teaching structure across all 61 chapters. Five numbered sections connect concrete problems, a small working implementation, mechanism and theory, execution traces, specific failures, alternatives, six independent exercises, an extension lab, and maintenance decisions.

## Delivered content

- 61 complete chapter demonstrations, 61 complete lab starters, and 61 complete reference solutions.
- 366 independent exercises with hints and explained answers: guided change, prediction, bug repair, design choice, extension, and justification.
- 66 diagram views and 64 focused code blocks with adjacent theory, exact filenames and line ranges, and authored line-by-line explanations.
- 122 glossary definitions and 1,049 recall cards.
- A concurrent capstone with a bounded queue, owned messages, two workers, stable result slots, input rejection, and normal/exceptional shutdown.
- All 1,775 original slide lessons, 61 original presentations and transcripts, 61 existing cases, and 1,836 original study IDs preserved.

The updated course has 2,691 study entries and 2,007 primary reading entries after paired source slides are grouped. The accepted Design Patterns curriculum is unchanged. Browser storage still uses the original key and topic IDs.

## Verification evidence

- `workshop-verification.json`: all 61 Systems demonstrations and Design Patterns foundations compile with GCC 13.3, C++17, strict warnings, and assertions, then match expected stdout exactly.
- `systems-lab-verification.json`: all 122 starter/solution programs compile and match their separate exact output fixtures; hashes tie each record to its delivered source.
- `systems-preservation.json`: unchanged original topic content, IDs, lecture bytes, and approved Design Patterns data.
- `systems-depth-coverage.json`: chapter, exercise, vocabulary, source, and diagram counts.
- `../tests/reading-text-results.json`: all 2,544 primary lessons preserve their complete prose when rendered.
- `systems-diagram-layout-check.json`: browser geometry checks for text staying within diagram blocks, with diagrams also visually inspected.
- `../tests/systems-depth-browser-results.json`: all 61 maps, exercise answers, UML/code views, 183 source variants, downloads, glossary, and mobile overflow.
- `../tests/systems-browser-results.json`: original lectures, slide navigation, separate notes, and backup restoration.
- `rebuild-verification.json`: standalone and full-content regeneration from a fresh source-package extraction, including output digest.

Run the exact commands in the root README to reproduce these checks. The static test suite checks every diagram node’s C++ lines against its chapter source, all focused line ranges, all embedded source/output fixtures, source transcript preservation, and the approved Design Patterns digest.

## Scope of the checks

The programs use only the C++17 standard library. Hardware and OS models are teaching models, not kernel or networking implementations. Concurrent examples exercise real standard-library synchronization, but deterministic tests do not enumerate every possible schedule or prove general thread safety. No performance result is inferred from these small fixtures. The supplied lecture package referred to original companion listings that were not supplied; those missing listings are not claimed as tested. Validation was performed on Linux with GCC and Chromium, not every operating system or compiler.

The rebuild reproduced the standalone HTML byte for byte using both the basic build and full content/diagram regeneration from a fresh ZIP extraction. Existing pinned dependencies were reused; network dependency installation was not repeated. All 16 content tests passed in the extracted copy.

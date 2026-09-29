# Current revision

The Systems parity revision is documented in [SYSTEMS_REVISION.md](SYSTEMS_REVISION.md), with the current lab, diagram, browser, preservation, and rebuild reports beside it. The records below describe earlier releases.

# Validation record

Validated on 27 September 2026.

- **C++:** GCC 13.3.0, C++17, `-Wall -Wextra -Wpedantic -Werror -pthread`. All 69 programs compiled and ran. Actual stdout matched the 69 supplied expected-output files exactly; all exited successfully without stderr. See `companion/verification.json`.
- **Content:** five automated content-test groups passed, checking all 24 chapter groups, named manuscript sections and chapter exercises, unique practice IDs, four choices and rationales for each scenario, source-line references, and all embedded program/output files.
- **UML:** 25 views rendered. All non-comment overview code fragments were found in the corresponding example source after whitespace normalization. All 51 focused code blocks matched their exact source lines, with an explanation for every displayed line. Relationship endpoints were validated. Overview views were visually inspected; code-box sizing and arrow-label spacing were corrected. The capstone includes class/ownership, state, and sequence views. See the editable diagram JSON, SVGs, and DOT files.
- **Browser:** headless Chromium through Playwright 1.62.1. Every chapter loaded in each applicable study mode. Native browser tests passed for saved notes and progress after reload, flashcard ratings/review, scenario feedback/retry, expanded diagrams, C++ variants/download, glossary and search recovery, mobile drawer and page width, backup export/restore, and absence of JavaScript errors. See `tests/browser-results.json`.
- **Layout:** inspected desktop and 390-pixel mobile screenshots, overview diagrams, focused code/theory panels, and capstone state/sequence views. Long code and diagrams scroll inside their panels. Reduced-motion rendering was used for still previews.
- **Rebuild and packaging:** the standalone app was rebuilt from an extracted source ZIP and compared byte-for-byte with the delivered HTML. The regenerated content/diagram build and content tests were also checked. ZIP CRC integrity was checked.

## Limits

Browser execution was checked in Chromium, not Safari or Firefox. The standard Playwright browser download endpoint was unavailable in this environment, so the browser checks used a separately installed Chromium executable; the test scripts also support Playwright's standard installation. No cloud publishing or external service is needed. Optional Clang and undefined-behavior sanitizer commands are documented but were not rerun for this app package. C++ executes locally through the supplied compiler/test commands, not inside the browser.

## Systems Programming addition (version 1.1)

- All 61 source decks were parsed, with 1,775 slide lessons and exactly matching transcript sections. The 61 original PPTX payloads and full transcript downloads match their supplied files.
- The new course includes 805 source-based recall cards and 61 original chapter-specific scenarios. It has 1,836 total lessons including the cases.
- Nine content-test groups cover both courses. Design Patterns source files and expected outputs were not changed by this addition; its earlier 69-program compiler verification still applies to those exact files. Lecture snippets were preserved as teaching material and were not represented as newly compiled complete programs.
- The additional browser report is `tests/systems-browser-results.json`. It checks all 61 chapters in all four applicable modes, separate course progress, source downloads, slide navigation, practice feedback, mobile layout, and restoring more than 1,000 reviewed lessons.

The integrated version was rebuilt from the extracted source ZIP using both the basic build and the complete content/diagram regeneration. Both produced byte-identical standalone HTML, and all nine content-test groups passed. Desktop and phone-sized Systems Programming views were visually inspected.

## Teaching revision (version 1.2, 28 September 2026)

- Both courses now have chapter → section → lesson hierarchies: **85 chapter maps, 436 named sections, and 85 connected workshops**. All 2,375 original source/case topic records retain their IDs. The 684 paired Deep Dive entries are linked to their primary lessons; 1,690 primary lesson explanations plus the glossary remain in the default reading sequence.
- **62 new complete C++17 programs** (61 systems workshops and one foundations program) compiled with GCC 13.3.0 and strict warnings. Every program's actual output exactly matched its expected-output file. See `docs/workshop-verification.json`. The existing 69 Design Patterns program files remain unchanged and retain their preceding verification.
- **13 automated content-test groups passed.** They check original coverage, exact code and source references, hierarchy coverage, workshop code and outputs, discussion presence, and the visible correction to the race-condition explanation.
- `scripts/check_reading_text.mjs` opened **all 1,690 primary reading lessons in Chromium** and checked every displayed explanation paragraph against its complete source text. This includes decimal values and C++ names; paragraph formatting must not discard them. See `tests/reading-text-results.json`.
- The new outline browser suite exercised every chapter's map, linked reading view, example, trace, output, discussion, source disclosure, and notes. It verified downloadable C++ contents, course-outline navigation, the race correction, mobile width, and separate persistent course notes. See `tests/teaching-browser-results.json`.
- Existing Design Patterns and Systems Programming browser regression suites passed with selectors scoped to the new nested navigation. They cover all chapters and modes, original lecture downloads, cards, scenarios, notes, search, diagrams, backups, imports, and more than 1,000 review records.
- Desktop and phone-sized chapter maps, numbered sections, code/output pairs, trace tables, and discussion answers were visually inspected. Long code and tables scroll within their panels on small screens.
- The final ZIP passed CRC integrity. Both the basic build and full content/diagram regeneration from its extracted files produced a byte-identical standalone HTML. Source content, source diagrams, new workshop generators, dependencies, and verification scripts are included.

Limits remain: browser QA used Chromium, not Firefox or Safari; compiler QA used GCC, not every supported C++17 compiler. Hardware and OS demonstrations labeled **model** were checked as C++ programs, not as implementations or measured predictions of a real machine. Original systems companion listings absent from the supplied lecture archive were not compiled. New independent workshop programs provide executable examples alongside those original sources.

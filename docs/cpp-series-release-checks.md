# C++ book integration — release 1.4

The supplied `CPP_Corrected_Part_1_Books_I-VIII.zip` is integrated into the existing offline Study Studio app. It adds eight courses while preserving the approved Design Patterns and Systems course payloads.

## Coverage

- 8 books, 186 numbered chapters, 243 reading maps including front/reference groups.
- 4,173 reading entries; 1,482 extracted source listings; 1,066 recall cards; 729 self-reviewed practice prompts.
- 8 authored diagram studies, each based on a Chapter 1 source example, with 16 focused code/theory panels total.
- Every EPUB spine text character retained apart from presentation whitespace. Independent XML comparison is in `cpp-series-text-coverage.json`.
- PDF/EPUB downloads match the supplied bytes by SHA-256. Extracted listings match their source blocks, with a final newline added where needed.
- Original courses match the full pre-integration canonical content hashes in `pre-series-approved.json`.

## Code audit

GCC 13.3.0 on Linux checked 590 C++/CUDA entry-point candidates under C++20. The report records 233 compiled/running cases without exact source fixtures, 112 exact output matches, 20 compiled cases requiring external fixtures, 5 output differences, 34 signal-wait timeouts, 156 independent compile failures, and 30 CUDA cases not validated. See `cpp-series-code-verification.json` for per-file evidence and diagnostics. No source repair is implied. Existing C++17 course programs are unchanged and retain their prior successful verification records.

Missing companion headers, external services, CUDA execution, OS/network fixtures, and infrastructure commands were not supplied or not executed. These limitations are visible in the code view. No browser-based C++ execution is claimed.

## Interface checks

Automated browser checks exercise every new reading map; source-code and expected-output rendering; original PDF/EPUB and source downloads; saved practice responses and self-review; flashcards; full-size diagrams; exact focused line references; source internal links; book export/import; and backup/restore. The app works as an offline file. Tests use Chromium, desktop 1512×1100 and mobile 390×844. Results are in `tests/series-browser-results.json` and `tests/series-advanced-browser-results.json`.

Visual inspection covers desktop chapter maps, code, practice, mobile reading/navigation, all eight overview diagrams, and representative focused theory panels. Long focused source lines wrap without changing their logical line references. Code catalogs use horizontal scrolling for complete listings; diagrams remain scrollable on narrow screens.

Basic and full content/diagram rebuild commands are documented in the root README. ZIP CRC integrity and a fresh extraction/rebuild are checked before delivery. The final `dist/index.html` matches the separately delivered standalone HTML.

## Navigation correction, version 1.4.1

Each of the ten books now has an independent, labeled chapter dropdown in the sidebar. The former combined book selector is removed. A dedicated browser check verifies chapter boundaries, final chapters, glossary choices, independent notes, saved reading positions, mobile drawer behavior, and personal material. See `tests/book-dropdown-browser-results.json`. The study data is unchanged.

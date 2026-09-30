# Before / after — Programming Studio 2.1

All sizes below are exact bytes.

| Measurement | Original | Refactored |
|---|---:|---:|
| index.html | 75,280,512 | 1,220 |
| Monolithic content.json | 74,422,433 | Absent; modular source files |
| Largest JS | Inline in HTML | 167,075 |
| Initial JS (both files) | Inline in HTML | 170,430 |
| CSS | Inline in HTML | 63,851 |
| Master catalog | Inline in HTML | 42,475 |
| Largest course catalog | Inline in HTML | 793,177 |
| Largest chapter | Inline in HTML | 479,760 |
| Total dist | 75,280,512 | 92,532,435 |
| Initial browser response bodies | 75,280,512 | 518,953 |
| Initial browser transfer | 75,280,812 | 521,053 |

Startup transfer fell 99.31%. The initial route is the default Design Patterns foundations chapter, in a fresh browser context. Both measurements use the same local, uncompressed HTTP serving conditions. `transferSize` is the browser Resource Timing measurement including its header allowance; body bytes are listed separately. Production GitHub compression/CDN behavior was not measured.

The Pages distribution is larger overall because it now also serves the original standalone source/build resources separately. These files, diagrams, source books and presentations are not fetched at startup. Largest generated file: `resources/teaching/systems-depth.json` (2,621,599 bytes). Largest chapter: `data/systems-programming/ch1.json`.

## Exact preservation counts

| Content measure | Before | After |
|---|---:|---:|
| courses | 10 | 10 |
| books | 10 | 10 |
| cppBooks | 8 | 8 |
| chapters | 328 | 328 |
| topicChapterGroups | 337 | 337 |
| lessons | 7,403 | 7,403 |
| readingEntries | 6,719 | 6,719 |
| sections | 1,923 | 1,923 |
| flashcards | 2,724 | 2,724 |
| practice | 839 | 839 |
| sourceListings | 1,482 | 1,482 |
| codeBlocks | 442 | 442 |
| exampleChapters | 84 | 84 |
| codingQuestions | 71 | 90 |
| workedPrograms | 164 | 230 |
| publicCodingChecks | 323 | 425 |
| diagramChapters | 92 | 92 |
| diagramImages | 99 | 99 |
| examplePrograms | 84 | 84 |
| labStarterPrograms | 84 | 84 |
| labSolutionPrograms | 84 | 84 |
| chapterWorkshopRecords | 85 | 85 |
| allCodeListingBlocks | 1,924 | 1,924 |
| lectureDecks | 61 | 61 |
| slideLessons | 1,775 | 1,775 |
| bookPDFs | 8 | 8 |
| bookEPUBs | 8 | 8 |
| personalMaterialWorkspaces | 1 | 1 |

`books = 10` counts both Design Patterns and Systems as book-like curricula; eight are the C++ textbook series. `chapters = 328` includes declared front/reference chapters. Nine additional glossary topic groups produce 337 navigable groups. `lessons = 7,403` counts every source topic, including paired deep dives; `readingEntries = 6,719` counts the curated reading-order entries. `codeBlocks = 442` is the original non-series code-block kind; the 1,482 series preformatted source listings bring all code-bearing blocks to 1,924. Resource categories overlap and must not be summed as distinct programs. The 2.1 Book III expansion deliberately adds 19 coding questions, 102 public checks and 66 runnable workshops. All original counts and content are preserved; those additions are validated separately in BOOK_III.md and the editorial baseline tests.

## Per-course coverage (before = after)

| Course | Chapters | Groups | Lessons | Reading entries | Cards | Practice |
|---|---:|---:|---:|---:|---:|---:|
| design-patterns-cpp | 24 | 25 | 539 | 539 | 609 | 49 |
| systems-programming | 61 | 62 | 2,691 | 2,007 | 1,049 | 61 |
| cpp-book-01 | 27 | 28 | 575 | 575 | 119 | 163 |
| cpp-book-02 | 29 | 29 | 621 | 621 | 96 | 69 |
| cpp-book-03 | 26 | 27 | 479 | 479 | 98 | 60 |
| cpp-book-04 | 27 | 28 | 481 | 481 | 110 | 63 |
| cpp-book-05 | 25 | 26 | 361 | 361 | 93 | 63 |
| cpp-book-06 | 30 | 31 | 413 | 413 | 106 | 67 |
| cpp-book-07 | 38 | 39 | 669 | 669 | 240 | 112 |
| cpp-book-08 | 41 | 42 | 574 | 574 | 204 | 132 |

Original reading content and all earlier coding items were checked with canonical SHA-256, not inferred from matching counts. The current additive coding catalog is also checked byte-equivalently against the generated output. See MIGRATION.md and the JSON reports for tests and limits.

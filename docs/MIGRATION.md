# Migration and validation record

## Input

The latest supplied/recovered Study Studio source archive was version 1.7.0, containing ten curricula and a personal-material workspace. Its `dist/` contained only the 75,280,512-byte self-contained index. Its authoritative `src/content.json` was 74,422,433 bytes. No Git repository metadata or Pages workflow was in that archive.

The migration imported all fields into chapter-oriented source files and decoded embedded image/book/presentation bytes into hash-named assets. It did not regenerate, condense or edit educational prose, examples or explanations. Original order and IDs are preserved.

## Checks performed

- Canonical reconstruction matches the original complete content and coding datasets exactly.
- All 30 previous content tests pass, including exact book/source/presentation bytes, diagram edges and focused line excerpts, original chapter coverage, cards/practice and code validation metadata.
- Four additional migration/resource tests compare all source/output fields, counts, search strings, navigation references, linked source listings and every emitted file hash.
- Normal build automatically checks current source/output identity and internal references.
- Browser tests render all 328 declared chapters and nine extra glossary groups (337 groups total).
- Every available study mode is exercised in all ten curricula: Outline, Flashcards, Practice/Scenarios, Code/Labs, UML, Coding Lab, and applicable Book/Lecture views.
- Explicit project-path URLs restore chapter and view on refresh; cache/network checks confirm chapter changes do not fetch other books.
- Search-only shards preserve the original substring search and load only for the selected book.
- Notes, bookmarks, reading marks, card ratings, practice history, review states and coding drafts survive reload.
- Backup/export/restore preserves the actual saved records; study-pack export materializes the full selected book and its binary assets.
- PDF and PowerPoint downloads succeed as separate static assets.
- Desktop and 390px mobile screenshots were inspected; the tested mobile pages have no horizontal document overflow.
- Coding Lab regression tests exercise passing/failing feedback, persisted results, malformed/rate-limit responses, cancellation, safe output rendering, editor keys and source navigation. A changed C++ program is compiled and run locally to establish its actual failed-check output. Remote compiler responses are simulated; no supplied source was sent to an external compiler during this refactor.
- The optional offline build is tested directly through `file://`, including Coding Lab state/backup behavior with simulated transport.

Machine-readable counts/hashes: `docs/migration/baseline.json`, `dist/integrity.json`. Network measurements: `docs/migration/original-network.json`, `tests/modular-browser-results.json`. Test outputs and screenshots are included in the project.

## Limits

The actual GitHub repository and public Pages deployment were not modified or tested remotely. The complete output is tested at the identical project subdirectory on a local HTTP server. The supplied Actions workflow retains `./dist` publication.

No live Compiler Explorer integration call was made. Existing CUDA, external-library/service and hardware-specific limitations remain exactly as recorded in the original examples. This architecture change does not claim a new full recompile of every advanced C++ listing; byte identity and the original detailed verification records are preserved.

Offline refresh of the modular site is not guaranteed. Complete offline use is supplied separately, so Pages startup remains small.

An independent directory with no `node_modules/`, `dist/`, or monolithic source files was rebuilt using the lockfile. All 3,744 output files (including the manifest) matched the first build byte for byte, and all 33 then-current content/integrity tests passed again; the final suite adds a 34th test enforcing all 3,249 source-resource hashes. Separately, all 3,249 original educational resource files in the input archive were compared by SHA-256 with the retained project files: zero mismatches. Failure/retry and delayed-course-switch tests also passed.

The complete release ZIP was also extracted into a fresh directory and rebuilt with the documented `npm ci`, `npm run build`, and `npm test` commands. All 3,744 output files matched and all 34 tests passed. ZIP CRC integrity checks passed for all three release archives.

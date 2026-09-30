# Architecture and extension guide

## Two different catalogs

`dist/data/catalog.json` contains only library navigation metadata: identifiers, titles, domains, chapter labels and a relative URL for each course catalog. It contains no lesson bodies, code, presentations or book bytes.

A course's `catalog.json` contains its topic/section navigation, stable card/practice IDs, listing metadata, original glossary and cross-reference map. Body fields remain in chapter JSON files. Keeping IDs in metadata lets the existing progress counters, global-within-book decks, filters and shuffled/review queues work before those bodies are fetched. Original glossary definitions are small and remain available throughout the book.

`src/loader.js` has a promise cache for fetched URLs and tracks hydrated courses/chapters. A selected chapter replaces its topic stubs by ID, fills chapter maps/examples/diagrams/lectures and replaces listing metadata by ID. The original rendering functions then receive the same complete objects they received before the refactor. A series block index is invalidated after chapter hydration or full-book export.

`renderAll()` uses a request generation counter so an obsolete asynchronous render cannot replace a newer view. The workspace becomes inert while its next content loads. Course selection has its own generation guard. Failed requests are evicted from the fetch cache and present a retry button; saved study data is retained.

## Lossless source representation

`content/manifest.json` stores root fields, course order, each course's base/chapter filenames, original topic/listing order and coding-item order. `source.json` stores course-level fields; chapter files store complete original topics and the corresponding teaching/examples/diagrams/lectures/listings. Reassembly restores the original ordering.

Binary strings are represented as `{"$asset":"assets/<sha256>.<extension>","prefix":"..."}`. Bytes are decoded only from canonical base64. `prefix` preserves a data-URI prefix or the empty prefix for a raw base64 field. No educational string is changed. The build copies these assets and the UI displays/downloads them by relative URL. The offline build and study-pack export restore the original strings.

The original source JSON's canonical SHA-256 and counts are preserved in `docs/migration/baseline.json`. Canonicalization sorts object keys and preserves arrays/strings/values. Tests reconstruct all generated chapters and binaries and compare every field, not just headline counts. Source files and generated listings retain their own original hashes too.

## Search without downloading the book

The old UI used a substring search over topic blocks, readings, IDs, titles, chapter titles, tasks, summaries and concepts. Replacing that with title-only search would lose functionality. The build therefore creates the exact same normalized search strings, in independent gzip-compressed JSON shards. Each shard is limited to roughly 400 KB of uncompressed text (except a legitimately larger individual entry); the browser uses the standard `DecompressionStream` API.

On the first search, only the current course's search shards load. They contain no embedded PDFs, slide decks, images, code-lab drivers or complete chapter objects. Results are topic IDs already present in navigation metadata. Opening a result hydrates its chapter. The strings retain existing phrase, symbol and code searches. An empty query triggers no search fetch. Modern browsers supporting JavaScript modules, fetch and DecompressionStream are required for the modular edition.

## URLs and state

All static URLs resolve against `document.baseURI`; none assumes the site is at `/`. The app writes query parameters for course, chapter, topic, view, lesson/map selection and active card/practice item. It restores explicit URLs ahead of saved reading positions. Browser popstate restores the route. Navigation updates the current address with `replaceState`, retaining the existing in-app navigation controls.

No storage migration is necessary: the existing key, version and item identities stay fixed. Import validation and HTML escaping are retained. The progress cleaner now keeps every valid collection entry instead of silently slicing long collections. Existing individual field-size/type checks still apply. Browser storage remains origin-specific; migration between file and HTTPS origins uses the existing backup flow.

## Build responsibilities

`npm run build` reassembles source in Node, creates metadata/search shards, copies chapter files/binary assets/original resources, combines the existing UI scripts into `app.js`, emits `loader.js` and CSS, writes the HTML shell and integrity manifest, and validates the output. It fails on unresolved navigation/listing references, missing source downloads, a source/output mismatch or a chapter at/above 5 MB. It never deletes content to meet a size budget.

`dist/integrity.json` inventories every other generated file with SHA-256 and byte size. It includes no timestamps, so independent builds can be compared byte for byte. The manifest does not list itself.

The original companion/build resources are copied under `dist/resources/` without build binaries/cache folders. They are also retained at their original paths in the project archive, so original commands and line references remain meaningful.

## Adding or extending material

1. Edit the complete chapter source in `content/<course>/ch<number>.json`. Keep existing IDs stable; add unique IDs for new topics/cards/prompts/blocks.
2. Update its course `source.json` for chapter labels, reading order or glossary/cross-reference metadata as needed. Add new topic/listing IDs to the matching order arrays in `content/manifest.json`.
3. For a new course, add its base, chapter files and coding file, then add a manifest course entry and coding-file/order entries. No application code is required.
4. Put binary resources in `content/assets/`, name them by their byte SHA-256, and use the asset marker. Keep original source/build files in their existing companion directories.
5. Run `npm run build`, `npm run validate`, and browser tests. The historical migration-baseline test intentionally detects editorial changes: review and explicitly update the baseline only when accepting those changes. Preserve the historical baseline as a release record.
6. If a chapter reaches the build's 5 MB guard, split its material into additional stable chapter/section groups with explicit navigation and ordering. Do not truncate it or raise the limit to work around GitHub limits. Current chapters are all below 0.5 MB.

The build's working memory may include the whole library; that is separate from browser startup. Growth adds independent static files, not inline bytes in HTML. Complete offline distribution and explicit whole-book exports may intentionally read all their requested material.

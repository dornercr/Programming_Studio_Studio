# Example-led outline revision

The visible reading experience now groups every supplied chapter into meaningful sections rather than exposing a flat list of slide text. Source records and their original IDs remain intact.

| Course | Chapter maps | Named sections | Reading entries | Source/case records retained |
| --- | ---: | ---: | ---: | ---: |
| Design Patterns in C++ | 24 | 120 | 539 | 539 |
| Systems Programming | 61 | 316 | 1152 | 1836 |

The 684 paired Deep Dive entries sit with their primary source lesson. They remain in the source, search results, downloads, and saved progress records. Reading navigation does not force a reader through a duplicate before advancing.

Each chapter has a concrete workshop with requirements, visible C++ and output, an explained state trace, a mistake to avoid, an alternative to compare, and a discussion answer. There are 62 new complete programs (61 systems chapters and one Design Patterns foundation), alongside the existing 69 verified Design Patterns programs. Systems models are explicitly identified and do not claim to implement the kernel or hardware being modeled.

The whole-course outline, chapter map, nested sidebar, lesson jump links, source disclosure, and practice links provide separate routes through the same curriculum. Long source paragraphs are broken into readable paragraphs; substantial original explanations remain accessible in full. Chapter 1 systems discussion prompts and answers are also shown at their own relevant lessons.

Existing topic IDs and the local storage key remain unchanged. The revised app preserves notes, reviewed flags, bookmarks, card records, and scenario records. As before, export a backup before changing the path of a locally opened HTML file.

## Verification

- Every chapter map and representative lesson in every chapter was exercised in Chromium.
- All 62 new complete C++17 programs compiled with strict warnings and matched their expected output exactly.
- Content tests verify every original source topic is mapped or explicitly paired, every new program matches the embedded listing, and all previous code/diagram references remain valid.
- Original PowerPoint and transcript downloads still match their supplied files byte-for-byte.
- Desktop and mobile layouts were rendered and visually inspected, including chapter maps, code, trace tables, and discussion answers.
- Rebuild and archive integrity checks are recorded in `VALIDATION.md`.

The original source programs absent from the supplied systems lecture ZIP were not compiled. New systems workshops are independent demonstrations; their limited contracts are stated alongside the code. No claims are made about performance on hardware not measured here.

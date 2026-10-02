# Linux System Administration in Study Studio

This addition imports the supplied **Linux System Administration** EPUB by
Dr. Charles Dorner. It preserves the complete original reading and examples.
The introductory documents are combined in “Before you begin”; the 33 main
chapters, four appendices and references follow in their original order.

Included:

- 39 chapter/reference decks with 1,615 slides and downloadable transcripts.
- All 490 numbered sections, 3,503 original paragraphs, 735 original command,
  configuration and illustrative trace blocks, and the original EPUB download.
- Chapter maps, exact worked examples, operational diagrams, recall cards,
  chapter decision cases, bookmarks, notes and review progress.
- A local Bash fixture lab for each main chapter, with a starter, checked
  solution, assertions, expected output and automatic temporary-directory cleanup.
- Diagrams that fit the normal reading pane and a viewport-sized viewer with
  Fit screen, zoom buttons and mouse-wheel zoom inside a bounded scroll area.

Open the usual `http://localhost:5173` address. Choose **Linux System
Administration** in the book menu and choose a chapter. Use **Outline** for the
discussion and worked example, **Slides** for the lecture, **Commands** for the
original source examples, **Diagrams** for the visual model, and **Linux labs**
for the editable local exercise. **Book** downloads the original EPUB.

Linux text is handled separately from the C++ executor. Commands in the book
remain examples with their original environment, preconditions, verification
and recovery instructions. Fixture labs are explicitly marked when they model
systemd, storage, SELinux, networking or cloud state; they do not claim to operate
those real privileged services. A browser does not execute a host shell.

## Install

Save `Linux_System_Administration_Study_Studio.patch` in Documents, then run:

```bash
cd "$HOME/Downloads/Design_Patterns_Study_Studio_Source/Design_Patterns_Study_Studio" &&
(
  if [ ! -f scripts/install-linux-admin.mjs ]; then
    git apply --check "$HOME/Documents/Linux_System_Administration_Study_Studio.patch" &&
    git apply "$HOME/Documents/Linux_System_Administration_Study_Studio.patch" || exit 1
  fi
  node scripts/install-linux-admin.mjs &&
  npm run build &&
  npm test &&
  npm start
)
```

The patch adds new files only. The installer checks all shared edit locations
and JavaScript syntax before writing, merges only the Linux course and lecture
registrations, and backs up changed files under `backups/linux-admin-TIMESTAMP`.
It keeps the existing builders and other course registrations. Running the
installer again is safe and does not duplicate the course or renderer.

`npm run build:offline` includes the course, diagrams, EPUB and lectures in the
portable build. Local fixture lab solutions can be run from the source root with
`bash linux-labs/chNN.sh`; downloaded edits use `bash -n linux-lab.sh && bash
linux-lab.sh`. Bash syntax checking alone does not establish runtime correctness.

## Verification

The original regression suite and the added Linux tests pass. Both builds
succeed, including a project with the earlier Systems, Design Patterns, Book I,
Book II discussion additions and desktop sidebar toggle. The added checks cover reading and example counts,
source/lecture coverage, exact example bytes, EPUB identity, chapter mapping,
local lab syntax, assertions and exact output, and Linux renderer interactions.
The direct EPUB import comparison preserves every original paragraph and
preformatted block. Source-to-build, search and asset integrity tests include
the Linux course. Only historical pre-addition whole-library hash comparisons
exclude its explicit course ID; their original expected hashes stay unchanged.

Browser layout was not exercised with a real browser in this environment.

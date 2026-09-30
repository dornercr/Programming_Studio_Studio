# C++ Coding Lab

71 original practice questions: 23 for Book I, 24 for Book II, and three for each of the other eight curricula. The 86 Book I workshops cover its 92 C++ listings; the 78 Book II workshops cover its 84 C++ listings. Both sets join supporting code where required and preserve the exact original source alongside a tested single-file version. C++20, standard library, POSIX threads.

Each question directory contains:

- `question.json`: requirements, hints, explanation, public test expressions, expected values, and the matching book chapter.
- `starter.cpp`: editable function or class with an intentional omission or bug.
- `solution.cpp`: the explained reference implementation.
- `driver.cpp`: `main()` for sample or custom stdin.

The function/class files are not standalone programs. Join one with its driver, or use the app's Download .cpp button to receive a complete program. Every starter compiles with its driver but deliberately fails one or more teaching checks.

From the source ZIP root:

```sh
mkdir -p coding_lab/build
cat coding_lab/b1-add/solution.cpp coding_lab/b1-add/driver.cpp > coding_lab/build/add.cpp
g++ -std=c++20 -Wall -Wextra -pedantic -pthread coding_lab/build/add.cpp -o coding_lab/build/add
printf '7 5\n' | coding_lab/build/add
# Output: 12
```

To compile all references, compare every sample output, run 323 question checks, and check that the starters expose a failure:

```sh
node scripts/test_coding_questions.mjs
```

See `tests/coding-question-results.json` for the delivered verification. `node scripts/test_book_one_worked.mjs` compiles the 86 Book I workshops and checks 90 cases; `node scripts/test_book_two_worked.mjs` compiles the 78 Book II workshops and checks 81 cases. Both `book_01_worked/<listing-id>/` and `book_02_worked/<listing-id>/` contain `original.cpp`, runnable `main.cpp`, and `workshop.json`. Single-file adaptations are marked in the manifest. C++ test harnesses come from `src/coding-core.mjs`; no third-party test library is required. Node.js 20+ and GCC with C++20 support are required for local checks.

Edit the authoritative question definitions in `scripts/make_coding_questions.py` and `scripts/book_one_coding.py`; source-linked workshops are assembled in `scripts/book_one_worked.py`, then regenerate:

```sh
python3 scripts/make_coding_questions.py
node scripts/build.mjs
```

The standalone app's Run and Check commands use Compiler Explorer online. These public checks are a teaching aid, not a secure grader or a guarantee of correctness for every input or thread schedule.

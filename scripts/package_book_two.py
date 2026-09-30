#!/usr/bin/env python3
"""Create the standalone, source-linked Book II C++ companion ZIP."""
from source_store import load_content, load_coding
from pathlib import Path
import json,zipfile,shutil,tempfile,sys
ROOT=Path(__file__).resolve().parents[1]
OUTPUT=Path(sys.argv[1]) if len(sys.argv)>1 else ROOT.parent/'output/Book_II_CPP_Coding_Workshops.zip'
data=load_coding()
catalog={'version':1,'book':'Book II · Modern C++ Programming','standard':'c++20',
         'questions':[q for q in data['questions'] if q['courseId']=='cpp-book-02'],
         'workedPrograms':[w for w in data['workedPrograms'] if w['courseId']=='cpp-book-02']}
assert len(catalog['questions'])==24 and len(catalog['workedPrograms'])==78
with tempfile.TemporaryDirectory(prefix='book_two_companion_') as scratch:
 base=Path(scratch)/'Book_II_CPP_Coding_Workshops';base.mkdir()
 (base/'catalog.json').write_text(json.dumps(catalog,indent=2,ensure_ascii=False)+'\n')
 shutil.copy2(ROOT/'src/coding-core.mjs',base/'coding-core.mjs')
 shutil.copy2(ROOT/'scripts/book_two_companion_verify.mjs',base/'verify.mjs')
 for q in catalog['questions']:
  folder=base/'questions'/q['id'];folder.mkdir(parents=True)
  for name in ['starter.cpp','solution.cpp','driver.cpp','question.json']:
   shutil.copy2(ROOT/'coding_lab'/q['id']/name,folder/name)
 for w in catalog['workedPrograms']:
  folder=base/'worked'/w['sourceId'];folder.mkdir(parents=True)
  for name in ['main.cpp','original.cpp','workshop.json']:
   shutil.copy2(ROOT/'coding_lab/book_02_worked'/w['sourceId']/name,folder/name)
 (base/'README.md').write_text('''# Book II · Modern C++ Programming — C++20 coding workshops

Book credited to Dr. Charles Dorner. This standalone companion contains 24 graded questions with 104 checks and 78 source-linked runnable workshops with 81 behavior checks. They represent all 84 C++ listings in the supplied Book II: six shared header/implementation/test-header listings are joined to the programs that use them. `original.cpp` is the exact book listing; `main.cpp` is a runnable source file. Thirty-one workshop source files are clearly labeled adaptations, mostly because the book's test header or multi-file project components had to be joined. Two displays were stabilized: a clock tick count and library-dependent vector relocation counts. See `workshop.json` for the full source mapping and experiment guidance.

Requirements: Linux, Node.js 20+, and g++ with C++20 support; validated with GCC 13.3.0. Standard library and `-pthread` only. No npm dependencies or internet. From the extracted `Book_II_CPP_Coding_Workshops` folder:

```sh
node verify.mjs
```

This compiles each solution and starter with its driver/test harness, proves every starter exposes a failure, and compares each workshop's recorded input, stdout, stderr, and exit code. It uses temporary build directories and checks 104 question cases and 81 workshop cases. A passing suite checks selected behavior, not every possible input or library implementation.

Run one question by joining its implementation to its driver:

```sh
cat questions/b2-money-add/solution.cpp questions/b2-money-add/driver.cpp > /tmp/book-two-money.cpp
g++ -std=c++20 -Wall -Wextra -pedantic -pthread /tmp/book-two-money.cpp -o /tmp/book-two-money
printf '125 75\\n' | /tmp/book-two-money
# 200
```

Run a Book II original or labeled adaptation:

```sh
g++ -std=c++20 -Wall -Wextra -pedantic -pthread worked/B02-L0002/main.cpp -o /tmp/book-two-user-id
/tmp/book-two-user-id
# true
```

The full Study Studio also keeps the other nine books, chapter dropdowns, study material, and original Book II source listings. Its browser coding lab sends source and stdin to an online compiler only after the reader clicks Run or Check.
''')
 OUTPUT.parent.mkdir(parents=True,exist_ok=True)
 with zipfile.ZipFile(OUTPUT,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
  for file in sorted(base.rglob('*')):
   if file.is_file():z.write(file,file.relative_to(base.parent))
 with zipfile.ZipFile(OUTPUT) as z:assert z.testzip() is None;print(f'{OUTPUT}: {len(z.namelist())} files; CRC passed')

#!/usr/bin/env python3
"""Create a compact standalone/offline Book I C++ companion from the verified app."""
from source_store import load_content, load_coding
from pathlib import Path
import json,zipfile,shutil,tempfile,sys
root=Path(__file__).resolve().parents[1]
out=Path(sys.argv[1]) if len(sys.argv)>1 else root.parent/'output/Book_I_CPP_Coding_Workshops.zip'
data=load_coding()
book={'version':1,'book':'Book I · C++ Foundations','standard':'c++20','questions':[q for q in data['questions'] if q['courseId']=='cpp-book-01'],'workedPrograms':[w for w in data['workedPrograms'] if w['courseId']=='cpp-book-01']}
assert len(book['questions'])==23 and len(book['workedPrograms'])==86
with tempfile.TemporaryDirectory(prefix='book_one_workshops_') as tmp:
 base=Path(tmp)/'Book_I_CPP_Coding_Workshops';base.mkdir()
 (base/'catalog.json').write_text(json.dumps(book,indent=2,ensure_ascii=False)+'\n')
 shutil.copy2(root/'src/coding-core.mjs',base/'coding-core.mjs')
 shutil.copy2(root/'scripts/book_one_companion_verify.mjs',base/'verify.mjs')
 for q in book['questions']:
  target=base/'questions'/q['id'];target.mkdir(parents=True)
  for f in ['starter.cpp','solution.cpp','driver.cpp']:
   shutil.copy2(root/'coding_lab'/q['id']/f,target/f)
  shutil.copy2(root/'coding_lab'/q['id']/'question.json',target/'question.json')
 for w in book['workedPrograms']:
  target=base/'worked'/w['sourceId'];target.mkdir(parents=True)
  for f in ['main.cpp','original.cpp','workshop.json']:
   shutil.copy2(root/'coding_lab/book_01_worked'/w['sourceId']/f,target/f)
 (base/'README.md').write_text('''# Book I · C++ Foundations — Interactive Coding Companion

Source book credited to Dr. Charles Dorner. This companion adds original coding exercises and source-linked workshops for **Book I only**. It works entirely offline with Node.js 20+ and a GCC compiler with C++20 support (validated on GCC 13.3.0, Linux). No npm packages or network are required. The C++ source uses the standard library and `-pthread`.

The `questions/` folder has **23 repair/extension questions with 108 checks**: one in each core chapter plus three earlier questions. Each has a deliberately incomplete or broken `starter.cpp`, an explained `solution.cpp`, a `driver.cpp` for standard input, and a `question.json` with the requirement, hints, input domain, and public checks. The starter and solution are function/class definitions; join either with `driver.cpp` to make a full program.

The `worked/` folder has **86 runnable programs with 90 checked input/output/exit cases**, representing all **92 C++ listings** in the book: 78 complete programs, eight short excerpts with explicit wrappers, and six source pieces included with their program. For each listing, `original.cpp` is exactly the book excerpt, while `main.cpp` is the runnable program. Fourteen `main.cpp` files are clearly labeled single-file adaptations or excerpt wrappers. `workshop.json` records their source references and any changes needed to run the original pieces together. Build commands and output snippets remain in the original book; they are not C++ programs.

From the extracted `Book_I_CPP_Coding_Workshops` directory:

```sh
node verify.mjs
```

This creates a disposable `build/` directory, compiles and runs every reference, starter, and worked program, and checks the **108 question cases** plus **90 worked-program cases**. It verifies that every starter exposes a failure. Compile just one solution:

```sh
mkdir -p build
cat questions/b1-add/solution.cpp questions/b1-add/driver.cpp > build/add.cpp
g++ -std=c++20 -Wall -Wextra -pedantic -pthread build/add.cpp -o build/add
printf '7 5\\n' | build/add
# 12
```

A source-linked whole program is built directly:

```sh
g++ -std=c++20 -Wall -Wextra -pedantic -pthread worked/B01-L0003/main.cpp -o build/sensors
./build/sensors
```

These checks cover recorded behavior and the selected boundaries. They do not prove every possible input, performance, or thread schedule. The original book/source archive remains available separately.
''')
 out.parent.mkdir(parents=True,exist_ok=True)
 with zipfile.ZipFile(out,'w',compression=zipfile.ZIP_DEFLATED,compresslevel=9) as z:
  for p in sorted(base.rglob('*')):
   if p.is_file():z.write(p,p.relative_to(base.parent))
 with zipfile.ZipFile(out) as z:assert z.testzip() is None;print(f'{out}: {len(z.namelist())} files; CRC passed')

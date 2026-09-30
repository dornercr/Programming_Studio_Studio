#!/usr/bin/env python3
"""Package verified Book III exercises, original listings, and a multi-file project."""
from pathlib import Path
from tempfile import TemporaryDirectory
import json
import shutil
import sys
import zipfile
from source_store import load_content, load_coding

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = Path(sys.argv[1]).resolve() if len(sys.argv)>1 else ROOT.parent/'output/Book_III_CPP_Coding_Workshops.zip'
content, coding = load_content(), load_coding()
book = next(c for c in content['courses'] if c['id']=='cpp-book-03')
catalog = dict(version=1,book=book['title'],author='Dr. Charles Dorner',standard='c++20',
               questions=[q for q in coding['questions'] if q['courseId']==book['id']],
               workedPrograms=[w for w in coding['workedPrograms'] if w['courseId']==book['id']])
assert len(catalog['questions'])==22 and len(catalog['workedPrograms'])==66

def fence(code, language='text'): return '\n```'+language+'\n'+code.rstrip('\n')+'\n```\n'

with TemporaryDirectory(prefix='book_three_package_') as temp:
    base = Path(temp)/'Book_III_CPP_Coding_Workshops'; base.mkdir()
    (base/'catalog.json').write_text(json.dumps(catalog,indent=2,ensure_ascii=False)+'\n')
    shutil.copy2(ROOT/'src/coding-core.mjs',base/'coding-core.mjs')
    shutil.copy2(ROOT/'scripts/book_three_companion_verify.mjs',base/'verify.mjs')
    for q in catalog['questions']:
        folder = base/'questions'/q['id'];folder.mkdir(parents=True)
        for name in ['starter.cpp','solution.cpp','driver.cpp','question.json']:
            shutil.copy2(ROOT/'coding_lab'/q['id']/name,folder/name)
    for w in catalog['workedPrograms']:
        folder = base/'worked'/w['sourceId'];folder.mkdir(parents=True)
        for name in ['main.cpp','original.cpp','workshop.json']:
            shutil.copy2(ROOT/'coding_lab/book_03_worked'/w['sourceId']/name,folder/name)
    # Keep exact original filenames, including commands, text excerpts, and shared support.
    shutil.copytree(ROOT/'cpp_series/book_03',base/'cpp_series/book_03')
    header = base/'cpp_series/book_02/listings/L0110.cpp';header.parent.mkdir(parents=True)
    shutil.copy2(ROOT/'cpp_series/book_02/listings/L0110.cpp',header)
    project = base/'projects/harbor-routes';project.mkdir(parents=True)
    for src,dest in [('L0091.cpp','routes.hpp'),('L0092.cpp','routes.cpp'),('L0093.cpp','main.cpp'),('L0094.cpp','tests.cpp')]:
        shutil.copy2(ROOT/'cpp_series/book_03/listings'/src,project/dest)
    shutil.copy2(header,project/'course_test.hpp')
    (project/'README.md').write_text('''# HarborRoutes — original multi-file application

These are unchanged listings B03-L0091 through B03-L0094, with their intended filenames restored. The shared test header comes unchanged from B02-L0110. Build from this directory:

```sh
g++ -std=c++20 -Wall -Wextra -pedantic -pthread routes.cpp main.cpp -o harbor-routes
printf '4 4 0 3\\n0 1 8\\n0 2 1\\n2 1 1\\n1 3 1\\n' | ./harbor-routes
# distance=3
# path=0,2,1,3
g++ -std=c++20 -Wall -Wextra -pedantic -pthread routes.cpp tests.cpp -o routes-tests
./routes-tests
# PASS checks=<the count printed by the invariant suite>
```

Input begins with vertices, edges, source, target; then one from/to/weight triple per edge. Source and target are zero-based. Bounds and failure behavior are explained in original listings and the Chapter 23 study unit. The acceptance program compares routes against an independent all-pairs solver, verifies path witnesses, injects input failures, checks rollback, and tests partial output failure. This is a bounded teaching application, not an unrestricted production routing service.
''')
    report = json.loads((ROOT/'tests/book-three-verification.json').read_text())
    shutil.copy2(ROOT/'tests/book-three-verification.json',base/'verification.json')
    bad = {r['id']:r['starterSampleOutput'] for r in report['results'] if r['kind']=='question'}
    guide = ['# Book III — study and experiment guide\n\nDr. Charles Dorner\n\nStart with the problem, predict a trace, run an example, then repair the independent exercise. The original book’s entire reading material remains in Programming Studio. This companion adds runnable code and verification; it does not replace the textbook. Programs under `worked/` are complete. Function/class files under `questions/` are excerpts until joined with `driver.cpp`.\n']
    answers = ['# Book III — separate explained answer key\n\nWrite an answer before opening this file. A passing public suite checks the declared fixtures; it does not prove every input, complexity bound, allocation behavior, or hardware speed claim.\n']
    for q in sorted(catalog['questions'],key=lambda x:x['chapter']):
        chapter = q['chapter'];work = [w for w in catalog['workedPrograms'] if w['chapter']==chapter]
        g = work[0]['guide']
        guide += ['\n## Chapter '+str(chapter)+' — '+q['chapterTitle']+'\n',
                  '\n### Problem and requirements\n\n'+g['problem']+'\n',
                  '\n### Design discussion\n\n'+g['design']+'\n',
                  '\n### Invariant and concrete trace\n\n'+g['invariant']+'\n\n'+g['trace']+'\n',
                  '\n### Failure and maintenance\n\n'+g['failure']+'\n\n'+g['maintenance']+'\n',
                  '\n### Runnable worked examples\n\n']
        for w in work:
            folder = 'worked/'+w['sourceId']
            guide += ['- ['+w['sourceId']+' — '+w['title']+']('+folder+'/main.cpp). Original: `'+w['sourceFilename']+'`. '+('Labeled adapter: '+w['adaptationNote'] if w['adapted'] else 'Unchanged single-file program.')+'\n']
        full = next((w for w in work if w['sourceSupportIds']),work[0])
        guide += ['\nBuild one complete program from the companion root:\n',fence('g++ -std=c++20 -Wall -Wextra -pedantic -pthread worked/'+full['sourceId']+'/main.cpp -o /tmp/book-three-example\n/tmp/book-three-example','sh'),
                  '\nExpected stdout for its first case:\n',fence(full['sampleOutput'] or '(empty)'),
                  '\nFor stdin/error cases, use that workshop’s `workshop.json`: it gives exact input, stdout, stderr and process exit. Do not apply the empty-input command above to the HarborRoutes command-line program; use `projects/harbor-routes/README.md` for its input.\n',
                  '\n### Independent exercise — '+q['title']+'\n\n'+q['prompt']+'\n',fence(q['signature'],'cpp'),
                  '\nSample stdin:\n',fence(q['sampleInput'] or '(empty)'), '\nExpected stdout:\n',fence(q['sampleOutput']),
                  '\nThe deliberately incorrect starter actually produces:\n',fence(bad[q['id']]),
                  '\nPublic checks:\n\n'+''.join('- '+t['label']+': '+t['hint']+'\n' for t in q['tests']),
                  '\nHints:\n\n'+''.join(str(i+1)+'. '+h+'\n' for i,h in enumerate(q['hints'])),
                  '\n### Extend and justify\n\n'+full['experiment']+'\n\nExplain the requirement in one sentence. Identify one assumption your change relies on. Add a failing test before changing code. Compare your approach with a simpler alternative, then explain which invariant each test protects.\n']
        answers += ['\n## Chapter '+str(chapter)+' — '+q['title']+'\n\n'+q['explanation']+'\n\nComplete implementation excerpt (join with its supplied driver):\n',fence(q['solution'],'cpp'),
                    '\nBuild the complete solution from the companion root:\n',fence('cat questions/'+q['id']+'/solution.cpp questions/'+q['id']+'/driver.cpp > /tmp/book-three-solution.cpp\ng++ -std=c++20 -Wall -Wextra -pedantic -pthread /tmp/book-three-solution.cpp -o /tmp/book-three-solution','sh'),
                    '\nSupply the exercise’s sample stdin to reproduce its expected output. `node verify.mjs` runs the sample and every public test expression automatically. Alternate implementations are reasonable when they preserve the specified result, ownership, ordering and validation rules; performance or requested technique constraints still need code review.\n']
    (base/'STUDY_GUIDE.md').write_text(''.join(guide))
    (base/'ANSWER_KEY.md').write_text(''.join(answers))
    (base/'README.md').write_text('''# Book III · Data Structures and Algorithms in C++

Dr. Charles Dorner

22 independent coding exercises with 116 checks; 66 complete runnable workshops covering all 68 C++ listings, plus all 96 original source/command/text listings. The two HarborRoutes supporting listings are joined into its executable programs, and are also included unchanged in a real multi-file project. Every adaptation is labeled in `workshop.json`; `original.cpp` is preserved. A study guide provides problem/design/trace/failure/maintenance discussions and hints. A separate answer key explains every reference solution.

Requirements: Linux, Node.js >=20.11, g++ with C++20 support (validated with GCC 13.3.0). Standard library and POSIX threads only; no npm install, network, external test library or backend required for the companion.

From this extracted folder:

```sh
node verify.mjs
```

The verifier compiles all references and deliberately broken starters, compares each sample output, verifies 116 function/class checks and 71 workshop input/stdout/stderr/exit cases, and checks the actual incorrect output discussed in the answer explanations. It also builds both versions of the original multi-file routing project. Temporary build directories are cleaned automatically. Public tests are selected teaching checks, not exhaustive proofs.

Read `STUDY_GUIDE.md`, attempt the starter under `questions/`, and consult `ANSWER_KEY.md` only after tracing it. `verification.json` records the delivered full-project checks. The full Programming Studio project includes this material in the Book III Coding Lab and keeps all ten books, reading, original PDFs/EPUBs, UML, notes, progress, backup/import and other study tools. Its optional online compiler sends code only when you explicitly run or check it.

Three original displays have portable workshop alternatives: address offsets in elements, equal-key hash consistency rather than a library-specific hash integer, and a nonnegative timing result rather than a changing nanosecond measurement. Run the exact originals when studying byte addresses, numeric hashes or benchmark timings; their output can vary by platform. All other modifications join missing support without altering algorithm bodies.
''')
    OUTPUT.parent.mkdir(parents=True,exist_ok=True)
    with zipfile.ZipFile(OUTPUT,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
        for file in sorted(base.rglob('*')):
            if file.is_file():z.write(file,file.relative_to(base.parent))
    with zipfile.ZipFile(OUTPUT) as z:assert z.testzip() is None;print(str(OUTPUT)+': '+str(len(z.namelist()))+' files; CRC passed')

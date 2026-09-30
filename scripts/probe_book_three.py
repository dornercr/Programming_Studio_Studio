"""Compile Book III adapters and record reviewed, constrained baseline behavior."""
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from tempfile import TemporaryDirectory
import json
import re
import subprocess
from source_store import load_content
from book_three_worked import build_worked, ROOT, BASELINES

content = load_content()
course = next(c for c in content['courses'] if c['id'] == 'cpp-book-03')
sources = {l['id']:l for l in course['series']['listings']}
programs = build_worked(content)
ROUTES = [('distance=3\npath=0,2,1,3\n','',0), ('unreachable\n','',0),
          ('distance=0\npath=0\n','',0), ('','error: unsigned integer required\n',2),
          ('','error: trailing field\n',2), ('','error: missing field\n',2)]
STABLE = {'B03-L0012':'0 1 2 ', 'B03-L0029':'true\n',
          'B03-L0083':'sum=4999950000 median_nonnegative=1\n'}

def inspect(w):
    id = w['sourceId']
    with TemporaryDirectory(prefix='book_three_probe_') as temp:
        exe = Path(temp) / 'example'
        compiled = subprocess.run(['g++','-std=c++20','-Wall','-Wextra','-pedantic','-pthread',
                                   str(ROOT/'coding_lab/book_03_worked'/id/'main.cpp'),'-o',str(exe)],
                                  capture_output=True,text=True,timeout=45)
        if compiled.returncode: raise RuntimeError(id+' compile:\n'+compiled.stderr)
        cases = []
        for i, case in enumerate(w['checks']):
            observed = subprocess.run([str(exe)], input=case['input'], cwd=temp,
                                      capture_output=True,text=True,timeout=10)
            expected = ROUTES[i] if id == 'B03-L0093' else None
            if expected:
                assert (observed.stdout, observed.stderr, observed.returncode) == expected, (id, case['label'], observed)
            else:
                assert observed.returncode == 0 and not observed.stderr, (id, observed)
                if w['sourceSupportIds']:
                    assert re.search(r'(?:^|\n)PASS checks=[1-9][0-9]*\n$', observed.stdout), (id, observed.stdout)
                else:
                    expected_stdout = STABLE.get(id, sources[id].get('validation',{}).get('stdout'))
                    assert expected_stdout is not None, id+' needs independent fixture review'
                    assert observed.stdout == expected_stdout, (id, observed.stdout, expected_stdout)
            cases.append(dict(stdout=observed.stdout,stderr=observed.stderr,exitCode=observed.returncode))
        print(id+' verified: '+str(len(cases))+' case(s)', flush=True)
        return id, cases

with ThreadPoolExecutor(max_workers=4) as pool:
    results = dict(pool.map(inspect, programs))
BASELINES.write_text(json.dumps(results,indent=2,ensure_ascii=False)+'\n')
print('Reviewed baselines:', len(results), 'programs;', sum(map(len,results.values())), 'cases')

"""One-time audit tool: compile Book II source and report observed fixtures.

The release uses frozen book_two_baselines.json, never dynamically accepts
program output as a correct expected value during a verification run.
"""
from source_store import load_content, load_coding
import json,subprocess,tempfile,sys
from pathlib import Path
from book_two_worked import build_worked,ROOT

entries=build_worked(load_content())
observed={};fail=[]
with tempfile.TemporaryDirectory(prefix='book_two_probe_') as tmp:
 for e in entries:
  code=ROOT/'coding_lab/book_02_worked'/e['sourceId']/'main.cpp'
  binary=Path(tmp)/e['sourceId']
  result=subprocess.run(['g++','-std=c++20','-Wall','-Wextra','-pedantic','-pthread',str(code),'-o',str(binary)],capture_output=True,text=True,timeout=35)
  if result.returncode:
   fail.append([e['sourceId'],'compile',result.stderr[:1400]])
   print('COMPILE FAILED',e['sourceId'],result.stderr[:500],flush=True)
   continue
  cases=[]
  for case in e['checks']:
   try:
    with tempfile.TemporaryDirectory(prefix='case_',dir=tmp) as cwd:
     run=subprocess.run([str(binary)],input=case['input'],capture_output=True,text=True,cwd=cwd,timeout=8)
    cases.append(dict(stdout=run.stdout,stderr=run.stderr,exitCode=run.returncode))
    if run.returncode and 'Invalid' not in case['label'] and 'Duplicate' not in case['label']:
     fail.append([e['sourceId'],'run',run.stderr[:1400]])
   except subprocess.TimeoutExpired:
    fail.append([e['sourceId'],'timeout','']);continue
  observed[e['sourceId']]=cases
  print(e['sourceId'],'COMPILED',[(v['exitCode'],len(v['stdout']),len(v['stderr'])) for v in cases],flush=True)
out=ROOT/'tests/book-two-probe.json';out.write_text(json.dumps(dict(observed=observed,failures=fail),indent=2)+'\n')
print('DONE',len(observed),'programs',len(fail),'failures',out,flush=True)
if fail: sys.exit(1)

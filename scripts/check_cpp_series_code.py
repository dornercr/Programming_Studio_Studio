#!/usr/bin/env python3
"""Compile supplied complete C++ candidates; run bounded, local standard-library cases."""
import concurrent.futures,hashlib,json,os,re,resource,subprocess,tempfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
data=json.loads((ROOT/'src/content.json').read_text());entries=[e for c in data['courses'] if c.get('series') for e in c['series']['listings'] if e['completeCandidate']]
OUT=ROOT/'cpp_series/build';OUT.mkdir(exist_ok=True)
def limits():
 resource.setrlimit(resource.RLIMIT_CPU,(5,5));resource.setrlimit(resource.RLIMIT_AS,(768*1024*1024,768*1024*1024));resource.setrlimit(resource.RLIMIT_FSIZE,(8*1024*1024,8*1024*1024))
def check(e):
 p=ROOT/e['filename'];code=p.read_text();r=dict(id=e['id'],file=e['filename'],sha256=hashlib.sha256(p.read_bytes()).hexdigest(),status='not-tested')
 if e['language']=='cuda':r.update(status='requires-cuda',detail='CUDA listings are retained. nvcc/device execution was not validated in this environment.');return r
 binary=OUT/e['id'];sources=[str(p)];include=[]
 # Preserve explicitly labeled multi-file examples as actual separate files.
 labels=list(re.finditer(r'^//\s+([a-zA-Z0-9_./-]+\.(?:cpp|hpp|h))\s*$',code,re.M))
 if len(labels)>1 and labels[0].start()<5:
  project=ROOT/e['filename'].split('/listings/')[0]/'projects'/e['id'];sources=[]
  for i,m in enumerate(labels):
   name=Path(m[1])
   if name.is_absolute() or '..' in name.parts:raise ValueError('Invalid project filename')
   f=project/name;f.parent.mkdir(parents=True,exist_ok=True);f.write_text(code[m.end():labels[i+1].start() if i+1<len(labels) else len(code)].strip()+'\n')
   if name.suffix=='.cpp':sources.append(str(f))
  include=['-I'+str(project),'-I'+str(project/'include')];r['project']=str(project.relative_to(ROOT))
 cmd=[os.environ.get('CXX','g++'),'-std=c++20','-Wall','-Wextra','-Wpedantic','-pthread',*include,*sources,'-o',str(binary)]
 try:
  built=subprocess.run(cmd,text=True,capture_output=True,timeout=45)
  r['diagnostics']=built.stderr.replace(str(ROOT)+'/', '')[:8000]
  r['buildCommand']=' '.join(x.replace(str(ROOT)+'/','') for x in cmd)
  if built.returncode:r.update(status='needs-context-or-repair',detail='The supplied listing did not compile independently. Read the compiler diagnostics; the source has not been silently changed.');return r
  r['status']='compiled'
  hazards=r'\b(?:system|popen|exec\w*|fork|socket|connect|bind|listen|kill|raise)\s*\(|(?:<|\")(?:(?:sys|netinet|arpa)/|unistd\.h)|std::cin|std::signal|scanf\s*\('
  if re.search(hazards,code):r['detail']='Compiled; execution needs input, an OS/process/network fixture, or a deliberate isolated failure exercise.';return r
  if re.search(r'\bargv\b',code):r['detail']='Compiled; the program requires its documented command-line fixture.';return r
  with tempfile.TemporaryDirectory(prefix=e['id']+'-') as cwd:
   run=subprocess.run([str(binary.resolve())],input='',text=True,capture_output=True,timeout=7,cwd=cwd,preexec_fn=limits)
  r.update(exitCode=run.returncode,stdout=run.stdout[:20000],stderr=run.stderr[:6000])
  if run.returncode:r.update(status='fixture-or-runtime-failure',detail='The executable did not complete successfully with an empty-input temporary-directory fixture. Consult its source requirements.');return r
  r['status']='ran'
  if 'expectedExact' in e:
   r['expectedMatch']=run.stdout==e['expectedExact'];r['status']='verified' if r['expectedMatch'] else 'output-differs'
   if not r['expectedMatch']:r['detail']='Actual output differs from the source’s printed expected-output block. Both are shown; the source was retained unchanged.'
  else:r['detail']='Compiled and ran successfully. The book describes expected behavior in prose; the captured output is an observation, not a claimed byte-for-byte source fixture.'
 except subprocess.TimeoutExpired:r.update(status='timeout',detail='The build or isolated run exceeded its time limit; no successful execution is claimed.')
 except Exception as ex:r.update(status='check-error',detail=str(ex))
 return r
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
 results=list(pool.map(check,entries))
report=dict(compiler=subprocess.check_output([os.environ.get('CXX','g++'),'--version'],text=True).splitlines()[0],standard='C++20',results=results)
(ROOT/'docs/cpp-series-code-verification.json').write_text(json.dumps(report,indent=2)+'\n')
from collections import Counter
print(dict(Counter(r['status'] for r in results)))

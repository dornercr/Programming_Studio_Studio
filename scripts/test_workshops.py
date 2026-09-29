#!/usr/bin/env python3
"""Compile each independent teaching program; compare actual stdout with its fixture."""
import concurrent.futures,json,os,subprocess
from pathlib import Path
root=Path(__file__).resolve().parents[1];out=root/'systems_examples/build';out.mkdir(exist_ok=True)
files=sorted((root/'systems_examples').glob('ch*/main.cpp'))+[root/'companion/foundations/main.cpp']
def check(p):
 name=p.parent.name if p.parent.name!='foundations' else 'foundations'
 target=out/name
 subprocess.run([os.environ.get('CXX','g++'),'-std=c++17','-Wall','-Wextra','-Wpedantic','-Werror','-pthread',str(p),'-o',str(target)],check=True,capture_output=True,text=True)
 r=subprocess.run([str(target)],check=True,capture_output=True,text=True,timeout=15)
 expected=p.with_name('expected.txt').read_text()
 assert r.stdout==expected,(name,r.stdout,expected)
 return dict(file=str(p.relative_to(root)),compiled=True,outputMatch=True)
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:results=list(pool.map(check,files))
(root/'docs/workshop-verification.json').write_text(json.dumps(dict(compiler=subprocess.check_output([os.environ.get('CXX','g++'),'--version'],text=True).splitlines()[0],standard='C++17',programs=results),indent=2)+'\n')
print(f'{len(results)} programs compiled and matched exact expected output.')

#!/usr/bin/env python3
"""Compile and run every Systems lab; assertions remain enabled."""
import concurrent.futures,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
OUT=ROOT/'systems_labs/build';OUT.mkdir(exist_ok=True)
FILES=sorted((ROOT/'systems_labs').glob('ch*/*.cpp'))
def check(p):
    binary=OUT/(p.parent.name+'_'+p.stem)
    result={'file':str(p.relative_to(ROOT)),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
    try:
        built=subprocess.run([os.environ.get('CXX','g++'),'-std=c++17','-Wall','-Wextra','-Wpedantic','-Werror','-pthread',str(p),'-o',str(binary)],capture_output=True,text=True,timeout=60)
        if built.returncode: raise RuntimeError(built.stderr)
        run=subprocess.run([str(binary)],capture_output=True,text=True,timeout=10)
        expected=p.with_name(p.stem+'_expected.txt').read_text()
        if run.returncode or run.stdout!=expected: raise RuntimeError(repr((run.returncode,run.stdout,expected,run.stderr)))
        result.update(compiled=True,outputMatch=True)
    except Exception as e: result['error']=str(e)
    return result
with concurrent.futures.ThreadPoolExecutor(max_workers=4) as pool:
    results=list(pool.map(check,FILES))
(ROOT/'docs/systems-lab-verification.json').write_text(json.dumps({'compiler':subprocess.check_output([os.environ.get('CXX','g++'),'--version'],text=True).splitlines()[0],'standard':'C++17','programs':results},indent=2)+'\n')
failed=[r for r in results if 'error' in r]
print(json.dumps({'programs':len(results),'failed':failed},indent=2))
raise SystemExit(bool(failed))

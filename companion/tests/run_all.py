#!/usr/bin/env python3
"""Compile independent chapter programs and compare their real stdout to fixtures."""
from __future__ import annotations
import argparse, concurrent.futures, difflib, json, os, shlex, subprocess, sys, time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]

def run_case(source,compiler,sanitize,build):
    rel=source.relative_to(ROOT);target=build/('_'.join(rel.parts[:-1]))
    flags=['-std=c++17','-Wall','-Wextra','-Wpedantic','-Werror','-pthread']
    if sanitize:flags+=['-O1','-g','-fsanitize=undefined','-fno-sanitize-recover=all','-fno-omit-frame-pointer']
    cmd=compiler+flags+[str(source),'-o',str(target)]
    started=time.monotonic()
    try:
        result=subprocess.run(cmd,capture_output=True,text=True,timeout=90)
        if result.returncode:
            return {'case':str(rel),'status':'FAIL','phase':'compile','details':result.stdout+result.stderr}
        actual=subprocess.run([str(target)],capture_output=True,text=True,timeout=10)
        expected=source.with_name('expected.txt').read_text()
        if actual.returncode!=0 or actual.stderr or actual.stdout!=expected:
            diff=''.join(difflib.unified_diff(expected.splitlines(True),actual.stdout.splitlines(True),fromfile='expected',tofile='actual'))
            return {'case':str(rel),'status':'FAIL','phase':'run','exit':actual.returncode,'stderr':actual.stderr,'details':diff}
        return {'case':str(rel),'status':'PASS','seconds':round(time.monotonic()-started,3),'stdout':actual.stdout}
    except (OSError,subprocess.TimeoutExpired) as e:
        return {'case':str(rel),'status':'FAIL','details':str(e)}

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--kind',choices=['examples','exercises','solutions','all'],default='all')
    ap.add_argument('--chapter');ap.add_argument('--sanitize',action='store_true');ap.add_argument('--jobs',type=int,default=4)
    ap.add_argument('--report',default=None);args=ap.parse_args()
    compiler=shlex.split(os.environ.get('CXX','g++'))
    version=subprocess.run(compiler+['--version'],capture_output=True,text=True,check=True).stdout.splitlines()[0]
    kinds=['examples','exercises','solutions'] if args.kind=='all' else [args.kind]
    sources=[]
    for k in kinds:sources += sorted((ROOT/k).glob(('ch'+args.chapter.zfill(2) if args.chapter else 'ch[0-9][0-9]')+'/main.cpp'))
    if not sources:raise SystemExit('No cases match the selection')
    build=ROOT/'build'/('ubsan' if args.sanitize else 'regular');build.mkdir(parents=True,exist_ok=True)
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1,args.jobs)) as pool:
        results=list(pool.map(lambda s:run_case(s,compiler,args.sanitize,build),sources))
    for r in results:
        print(r['status']+' '+r['case'])
        if r['status']!='PASS':print(r)
    report={'compiler':version,'standard':'C++17','sanitizer':'undefined' if args.sanitize else None,
            'cases':len(results),'passed':sum(x['status']=='PASS' for x in results),'results':results}
    rp=Path(args.report) if args.report else ROOT/('verification_ubsan.json' if args.sanitize else 'verification.json')
    rp.parent.mkdir(parents=True,exist_ok=True);rp.write_text(json.dumps(report,indent=2)+'\n')
    print(f'{report["passed"]}/{len(results)} programs passed; report: {rp}')
    return 0 if report['passed']==len(results) else 1
if __name__=='__main__':sys.exit(main())

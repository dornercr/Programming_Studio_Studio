#!/usr/bin/env python3
"""Run one input/output contract in a fresh temporary working directory."""
from __future__ import annotations
import argparse, json, subprocess, tempfile
from pathlib import Path

def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--exe', type=Path, required=True)
    parser.add_argument('--cases', type=Path, required=True)
    parser.add_argument('--index', type=int, required=True)
    args = parser.parse_args()
    case = json.loads(args.cases.read_text(encoding='utf-8'))[args.index]
    exe = args.exe.resolve(strict=True)
    with tempfile.TemporaryDirectory(prefix='cpp-course-') as temp:
        for name, text in case.get('files', {}).items():
            target = Path(temp) / name
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(text, encoding='utf-8')
        result = subprocess.run([str(exe), *case.get('args', [])],
                                input=case.get('stdin', ''), text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                cwd=temp, timeout=30, check=False)
    problems = []
    if result.returncode != case.get('exit', 0):
        problems.append(f'exit {result.returncode}, expected {case.get("exit", 0)}')
    if 'stdout' in case and result.stdout != case['stdout']:
        problems.append(f'stdout {result.stdout!r}, expected {case["stdout"]!r}')
    for text in case.get('contains', []):
        if text not in result.stdout: problems.append(f'missing stdout text: {text!r}')
    if 'stderr_contains' in case and case['stderr_contains'] not in result.stderr:
        problems.append(f'missing stderr text: {case["stderr_contains"]!r}')
    print(f'CASE {case["name"]}: exit={result.returncode}')
    print(result.stdout, end='')
    if result.stderr: print('STDERR:\n' + result.stderr, end='')
    if problems:
        print('\nFAIL: ' + '; '.join(problems))
        return 1
    print('CONTRACT PASS')
    return 0

if __name__ == '__main__':
    raise SystemExit(main())

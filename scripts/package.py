#!/usr/bin/env python3
"""Package a clean, editable offline app without caches or compiled binaries."""
from pathlib import Path
import argparse,zipfile
root=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('output',type=Path);args=p.parse_args()
exclude_names={'node_modules','__pycache__','.DS_Store','build'}
exclude_files={'adapt_app.py','extension.js','probe.mjs','temporary-backup.json','probe-mobile.png'}
args.output.parent.mkdir(parents=True,exist_ok=True)
with zipfile.ZipFile(args.output,'w',zipfile.ZIP_DEFLATED,compresslevel=9) as z:
 for f in sorted(root.rglob('*')):
  rel=f.relative_to(root)
  if f.is_symlink() or not f.is_file() or any(x in exclude_names for x in rel.parts) or f.name in exclude_files or 'diagram-check' in rel.parts:continue
  if f.resolve()==args.output.resolve():continue
  z.write(f,Path('Design_Patterns_Study_Studio')/rel)
with zipfile.ZipFile(args.output) as z:
 assert z.testzip() is None
 print(f'{args.output}: {len(z.namelist())} files; CRC check passed')

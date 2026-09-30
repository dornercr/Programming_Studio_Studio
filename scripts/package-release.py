"""Create the complete project, Pages-only and optional offline release archives."""
from pathlib import Path
import zipfile,json
ROOT=Path(__file__).resolve().parents[1]
OUT=Path(__import__('sys').argv[1]).resolve() if len(__import__('sys').argv)>1 else ROOT.parent/'release'
OUT.mkdir(parents=True,exist_ok=True)
def pack(target,entries):
    with zipfile.ZipFile(target,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
        for p,name in entries:z.write(p,name)
    with zipfile.ZipFile(target) as z:
        bad=z.testzip()
        if bad:raise RuntimeError('Corrupt ZIP member '+bad)
    return {'file':target.name,'bytes':target.stat().st_size,'zipIntegrity':'passed'}
files=[]
for p in sorted(ROOT.rglob('*')):
    rel=p.relative_to(ROOT)
    if p.is_file() and not set(rel.parts)&{'node_modules','dist-offline','build','__pycache__','.git'} and not rel.name.endswith('.pyc'):
        files.append((p,'Design_Patterns_Study_Studio/'+rel.as_posix()))
results=[pack(OUT/'Design_Patterns_Study_Studio_Source.zip',files)]
results.append(pack(OUT/'Programming_Studio_Pages.zip',[(p,p.relative_to(ROOT/'dist').as_posix()) for p in sorted((ROOT/'dist').rglob('*')) if p.is_file()]))
offline=ROOT/'dist-offline';(offline/'README.txt').write_text('Programming Studio — complete offline edition\n\nExtract this ZIP, then open index.html in your browser.\nAll books, chapters, diagrams, original book/lecture downloads and study tools are embedded.\nReading and saved study data work offline. The optional online C++ compiler needs internet.\nExport a backup before changing browsers or moving from file:// to the hosted website.\nUse the separate complete project ZIP for editable chapter sources, tests and rebuild instructions.\n')
results.append(pack(OUT/'Programming_Studio_Offline.zip',[(p,p.name) for p in sorted(offline.iterdir()) if p.is_file()]))
(OUT/'Programming_Studio_Migration_Report.txt').write_text((ROOT/'docs/migration/SIZE_REPORT.md').read_text()+'\n\n'+(ROOT/'docs/MIGRATION.md').read_text())
(OUT/'release-checks.json').write_text(json.dumps(results,indent=2));print(json.dumps(results,indent=2))

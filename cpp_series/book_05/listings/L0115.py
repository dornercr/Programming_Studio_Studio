#!/usr/bin/env python3
"""Build, test, stage, hash, and archive the real application. No network access."""
from pathlib import Path
import argparse
import hashlib
import json
import platform
import shutil
import subprocess
parser=argparse.ArgumentParser()
parser.add_argument("destination",type=Path,help="new directory; existing paths are refused")
args=parser.parse_args()
out=args.destination.resolve()
out.mkdir(parents=True,exist_ok=False)
source=Path(__file__).resolve().parent
build=out/"build"
stage=out/"stage"
commands=[
    ["cmake","-S",str(source),"-B",str(build),"-DCMAKE_BUILD_TYPE=Release"],
    ["cmake","--build",str(build),"--config","Release","--parallel","2"],
    ["ctest","--test-dir",str(build),"-C","Release","--output-on-failure"],
    ["cmake","--install",str(build),"--config","Release","--prefix",str(stage)],
]
with (out/"release.log").open("w",encoding="utf-8") as log:
    for command in commands:
        log.write("COMMAND: "+repr(command)+"\n"); log.flush()
        subprocess.run(command,check=True,stdout=log,stderr=subprocess.STDOUT,text=True)
manifest={"application":"HarborMetrics","version":"0.1.0","platform":platform.platform(),"files":[]}
for path in sorted(stage.rglob("*")):
    if path.is_file():
        manifest["files"].append({"path":str(path.relative_to(stage)),"sha256":hashlib.sha256(path.read_bytes()).hexdigest()})
(stage/"manifest.json").write_text(json.dumps(manifest,indent=2)+"\n",encoding="utf-8")
archive=shutil.make_archive(str(out/"harbor-metrics-0.1.0"),"zip",stage)
print(archive)

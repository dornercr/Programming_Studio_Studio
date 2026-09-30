#!/usr/bin/env python3
"""Check the exact vendored bytes against the reviewed local manifest."""
from pathlib import Path
import hashlib
import json
import sys
root = Path(__file__).resolve().parents[1]
manifest = json.loads((root / "dependencies.json").read_text(encoding="utf-8"))
for item in manifest["dependencies"]:
    path = (root / item["path"]).resolve()
    if not path.is_relative_to(root):
        raise SystemExit("dependency path escapes chapter")
    actual = hashlib.sha256(path.read_bytes()).hexdigest()
    if actual != item["sha256"]:
        raise SystemExit(f"integrity mismatch: {item['path']}")
    print(f"verified {item['name']} {item['version']}")
sys.exit(0)

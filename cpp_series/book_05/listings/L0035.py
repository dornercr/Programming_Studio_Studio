#!/usr/bin/env python3
"""Create and remove a temporary repository; never alters the caller's repository."""
from pathlib import Path
import shutil
import subprocess
import tempfile
if not shutil.which("git"):
    raise SystemExit("git is required for this optional exercise")
with tempfile.TemporaryDirectory(prefix="harbor-git-") as directory:
    root = Path(directory)
    def git(*arguments):
        return subprocess.run(["git", *arguments], cwd=root, check=True,
                              text=True, capture_output=True).stdout.strip()
    git("init", "--initial-branch=main")
    git("config", "user.name", "Harbor Teaching Fixture")
    git("config", "user.email", "fixture@example.invalid")
    (root / "contract.txt").write_text("count: 0..1000\n", encoding="utf-8")
    git("add", "contract.txt")
    git("commit", "-m", "Define bounded count contract")
    git("switch", "-c", "test-boundaries")
    (root / "tests.txt").write_text("accept 0 and 1000; reject 1001\n", encoding="utf-8")
    git("add", "tests.txt")
    git("commit", "-m", "Document count boundary cases")
    git("switch", "main")
    git("merge", "--ff-only", "test-boundaries")
    if git("status", "--porcelain"):
        raise SystemExit("unexpected dirty working tree")
    print(git("log", "--oneline", "-2"))
    print("PASS: temporary repository integrated with a clean tree")

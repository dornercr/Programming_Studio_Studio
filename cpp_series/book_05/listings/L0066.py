#!/usr/bin/env python3
"""Run an installed clang-tidy against this chapter and a real compilation database."""
import argparse
from pathlib import Path
import shutil
import subprocess
parser = argparse.ArgumentParser()
parser.add_argument("build_directory", type=Path)
args = parser.parse_args()
if not shutil.which("clang-tidy"):
    print("SKIP: clang-tidy is not installed")
    raise SystemExit(77)
if not (args.build_directory / "compile_commands.json").is_file():
    raise SystemExit("configure a supported generator with CMAKE_EXPORT_COMPILE_COMMANDS=ON")
source = Path(__file__).resolve().parents[1] / "main.cpp"
raise SystemExit(subprocess.run(["clang-tidy", str(source), "-p", str(args.build_directory)]).returncode)

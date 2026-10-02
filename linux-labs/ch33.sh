#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'content=pass\naccess=pass\ntls=pass\nrestore=missing\n' > acceptance.fixture
grep -qx 'restore=missing' acceptance.fixture
printf 'release held: restore rehearsal evidence missing\n'

#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'image=yes\nvolume=yes\nrole=no\n' > rebuild.fixture
grep -qx 'role=no' rebuild.fixture
printf 'instance image available; role dependency absent\n'

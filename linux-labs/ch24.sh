#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'installed=version-B\nrunning=version-A\n' > kernel.fixture
installed=$(sed -n 's/^installed=//p' kernel.fixture)
running=$(sed -n 's/^running=//p' kernel.fixture)
test "$installed" != "$running"
printf 'installed and running versions differ\n'

#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
mkdir source restored
printf 'release-7\n' > source/index
tar -cf backup.tar -C source .
tar -xf backup.tar -C restored
cmp source/index restored/index
printf 'restore matched source bytes\n'

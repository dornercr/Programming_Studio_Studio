#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
limit=256; observed=300
test "$observed" -gt "$limit"
printf 'fixture demand exceeds configured ceiling\n'

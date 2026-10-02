#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf '<h1>HarborStatus release-7</h1>\n' > page
grep -Fq 'HarborStatus release-7' page
if grep -Fq 'release-8' page; then exit 1; fi
printf 'approved marker present; wrong release rejected\n'

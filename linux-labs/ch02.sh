#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'one argument\n' > 'status report.txt'
name='status report.txt'
test "$(cat "$name")" = 'one argument'
set -- "$name"
test "$#" -eq 1
printf 'one path; one argument\n'

#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
old_lv=100; new_lv=140; fs_size=100
test "$new_lv" -gt "$old_lv"
test "$fs_size" -lt "$new_lv"
printf 'LV grew to 140; filesystem remains 100\n'

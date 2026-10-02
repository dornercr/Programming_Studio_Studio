#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
mkdir private
chmod 700 private
printf 'private\n' > private/note
chmod 600 private/note
test "$(stat -c %a private)" = 700
test "$(stat -c %a private/note)" = 600
printf 'directory=700; file=600\n'

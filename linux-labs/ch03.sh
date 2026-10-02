#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'retained\n' > original
ln original hardlink
ln -s original symlink
rm original
test "$(cat hardlink)" = retained
test -L symlink && test ! -e symlink
printf 'hard link retained; symbolic path broken\n'

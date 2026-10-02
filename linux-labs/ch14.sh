#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'boot=A unit=web result=ok\nboot=B unit=web result=failed\nboot=B unit=backup result=ok\n' > journal.fixture
selected=$(grep 'boot=B unit=web ' journal.fixture)
test "$selected" = 'boot=B unit=web result=failed'
printf '%s\n' "$selected"

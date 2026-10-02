#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'old\n' > status
printf 'new\n' > status.tmp
test "$(cat status)" = old
mv status.tmp status
test "$(cat status)" = new
printf 'old until replacement; new after replacement\n'

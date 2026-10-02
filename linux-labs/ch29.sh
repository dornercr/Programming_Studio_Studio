#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
before=release-7; candidate=release-8; observed=release-7
test "$observed" != "$candidate"
current=$before
printf 'acceptance failed; rollback selected: %s\n' "$current"

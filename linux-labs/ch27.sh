#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
apply() { if ! test -f config || ! grep -qx 'enabled=yes' config; then printf 'enabled=yes\n' > config; return 10; fi; }
first=0; apply || first=$?
second=0; apply || second=$?
test "$first" -eq 10 && test "$second" -eq 0
printf 'first run changed; second run unchanged\n'

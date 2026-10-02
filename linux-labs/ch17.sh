#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'fixture secret\n' > credential
chmod 644 credential
test "$(stat -c %a credential)" != 600
chmod 600 credential
test "$(stat -c %a credential)" = 600
printf 'broad mode detected; private mode verified\n'

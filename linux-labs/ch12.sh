#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'web1 key-A\n' > trusted.fixture
printf 'web1 key-B\n' > observed.fixture
if cmp -s trusted.fixture observed.fixture; then exit 1; fi
printf 'identity mismatch; investigate before trust\n'

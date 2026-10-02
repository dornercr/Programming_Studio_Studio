#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf '101 12 web\n102 80 worker\n103 5 logger\n' > processes.fixture
test "$(sort -k2,2nr processes.fixture | head -1)" = '102 80 worker'
printf 'highest fixture CPU: worker (80)\n'

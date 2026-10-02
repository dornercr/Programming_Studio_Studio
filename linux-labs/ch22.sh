#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'minute=1 cpu=20 latency=10\nminute=2 cpu=90 latency=200\n' > samples.fixture
selected=$(awk '/cpu=90/ {print $3}' samples.fixture)
test "$selected" = latency=200
printf 'pressure and latency coincide; cause still requires a test\n'

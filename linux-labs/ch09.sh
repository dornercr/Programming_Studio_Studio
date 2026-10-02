#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'bytes_free=1048576\ninodes_free=0\n' > capacity.fixture
. ./capacity.fixture
test "$bytes_free" -gt 0 && test "$inodes_free" -eq 0
printf 'bytes available; inode boundary exhausted\n'

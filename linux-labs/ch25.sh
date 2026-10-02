#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'definition=yes\ndisk=yes\nnetwork=no\n' > recovery.fixture
grep -qx 'network=no' recovery.fixture
printf 'disk exists; network dependency still missing\n'

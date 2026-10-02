#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'route=present\nlistener=absent\n' > network.fixture
grep -qx 'route=present' network.fixture
grep -qx 'listener=absent' network.fixture
printf 'route evidence present; listener still missing\n'

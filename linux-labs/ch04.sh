#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'operator:x:1200:1300:Operator:/home/operator:/bin/bash\n' > passwd.fixture
owner=$(awk -F: '$1=="operator" {print $3 ":" $4}' passwd.fixture)
test "$owner" = 1200:1300
printf 'operator -> %s\n' "$owner"

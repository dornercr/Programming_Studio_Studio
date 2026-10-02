#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'web1 baseline none\nweb2 exception unowned\n' > fleet.fixture
test "$(awk '$2=="exception" && $3=="unowned" {print $1}' fleet.fixture)" = web2
printf 'web2 exception needs an owner and expiry\n'

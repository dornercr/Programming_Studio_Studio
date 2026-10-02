#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'status.harbor.test 192.0.2.10\n' > dns.fixture
printf '192.0.2.10:443 unavailable\n' > endpoint.fixture
grep -q '192.0.2.10' dns.fixture
grep -q unavailable endpoint.fixture
printf 'name resolves; endpoint not established\n'

#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'symptom\nhypotheses\ntest\nrepair\nverification\nprevention\n' > packet
for field in symptom hypotheses test repair verification prevention; do grep -qx "$field" packet; done
printf 'six evidence fields present; review their substance\n'

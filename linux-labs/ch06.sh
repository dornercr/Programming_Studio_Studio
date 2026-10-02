#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'approved config\n' > config
sha256sum config > manifest
sha256sum -c manifest > /dev/null
printf 'changed\n' >> config
if sha256sum -c manifest > /dev/null 2>&1; then exit 1; fi
printf 'original verified; drift detected\n'

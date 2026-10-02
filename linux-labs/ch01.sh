#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
test -r /etc/os-release
test -n "$(uname -r)"
printf 'kernel observed; distribution file readable\n'

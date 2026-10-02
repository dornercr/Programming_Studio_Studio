#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
dac=allow; object_type=default_t; permitted_type=httpd_sys_content_t
test "$dac" = allow
test "$object_type" != "$permitted_type"
printf 'DAC permits; type mismatch remains\n'

#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
server_uid=1200; client_uid=2200
test "$server_uid" -ne "$client_uid"
printf 'same display name; numeric identity differs\n'

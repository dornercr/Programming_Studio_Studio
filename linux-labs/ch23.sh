#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'dns=ok\nroute=ok\nlistener=missing\n' > incident.fixture
grep -qx 'listener=missing' incident.fixture
printf 'next test: inspect listener and service state\n'

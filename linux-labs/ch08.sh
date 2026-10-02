#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
printf 'ActiveState=active\nUnitFileState=disabled\n' > unit.fixture
grep -qx 'ActiveState=active' unit.fixture
grep -qx 'UnitFileState=disabled' unit.fixture
printf 'running now; not enabled for boot\n'

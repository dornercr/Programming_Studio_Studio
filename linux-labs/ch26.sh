#!/usr/bin/env bash
set -euo pipefail
lab_dir=$(mktemp -d)
trap 'rm -rf -- "$lab_dir"' EXIT
cd -- "$lab_dir"
mkdir runtime durable
printf 'saved\n' > durable/state
printf 'ephemeral\n' > runtime/cache
rm runtime/cache
rmdir runtime
test "$(cat durable/state)" = saved
printf 'runtime removed; durable fixture retained\n'

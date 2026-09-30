#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/../../.." && pwd)
out=${1:?Usage: inspect.sh OUTPUT_DIRECTORY}
mkdir -p -- "$out"
compiler=${CXX:-c++}
"$compiler" -std=c++20 -O0 -S -I"$root/support" "$root/book05/ch14/main.cpp" -o "$out/unoptimized.s"
"$compiler" -std=c++20 -O3 -S -I"$root/support" "$root/book05/ch14/main.cpp" -o "$out/optimized.s"
printf '%s\n' "Assembly written to $out; compare the named functions, not just file sizes."

cmake -S source -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug --parallel 2
printf '1|2|0|Later\n3|1|1|Done\n2|1|0|First\n' \
  | ./build/debug/labs/harbor_reports --grouped

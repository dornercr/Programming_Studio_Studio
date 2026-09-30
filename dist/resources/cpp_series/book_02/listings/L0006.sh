cmake -S source/book2/ch01 -B build/b2-ch01 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch01 --parallel 2
ctest --test-dir build/b2-ch01 --output-on-failure

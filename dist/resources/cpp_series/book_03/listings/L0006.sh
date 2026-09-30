cmake -S source/book3/ch01 -B build/b3-ch01 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch01 --parallel 2
ctest --test-dir build/b3-ch01 --output-on-failure

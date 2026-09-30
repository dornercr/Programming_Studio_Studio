cmake -S source/book3/ch09 -B build/b3-ch09 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch09 --parallel 2
ctest --test-dir build/b3-ch09 --output-on-failure

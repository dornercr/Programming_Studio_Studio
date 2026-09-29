cmake -S source/book2/ch09 -B build/b2-ch09 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch09 --parallel 2
ctest --test-dir build/b2-ch09 --output-on-failure

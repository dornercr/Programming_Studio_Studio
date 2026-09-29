cmake -S source/book2/ch04 -B build/b2-ch04 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch04 --parallel 2
ctest --test-dir build/b2-ch04 --output-on-failure

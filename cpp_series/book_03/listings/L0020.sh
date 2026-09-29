cmake -S source/book3/ch04 -B build/b3-ch04 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch04 --parallel 2
ctest --test-dir build/b3-ch04 --output-on-failure

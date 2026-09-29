cmake -S source/book2/ch13 -B build/b2-ch13 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch13 --parallel 2
ctest --test-dir build/b2-ch13 --output-on-failure

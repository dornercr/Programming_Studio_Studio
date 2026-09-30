cmake -S source/book3/ch13 -B build/b3-ch13 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch13 --parallel 2
ctest --test-dir build/b3-ch13 --output-on-failure

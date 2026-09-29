cmake -S source/book3/ch18 -B build/b3-ch18 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch18 --parallel 2
ctest --test-dir build/b3-ch18 --output-on-failure

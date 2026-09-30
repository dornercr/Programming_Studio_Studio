cmake -S source/book2/ch18 -B build/b2-ch18 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch18 --parallel 2
ctest --test-dir build/b2-ch18 --output-on-failure

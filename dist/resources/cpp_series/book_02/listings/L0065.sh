cmake -S source/book2/ch15 -B build/b2-ch15 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch15 --parallel 2
ctest --test-dir build/b2-ch15 --output-on-failure

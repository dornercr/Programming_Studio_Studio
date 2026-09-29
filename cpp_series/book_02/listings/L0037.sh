cmake -S source/book2/ch08 -B build/b2-ch08 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch08 --parallel 2
ctest --test-dir build/b2-ch08 --output-on-failure

cmake -S source/book2/ch07 -B build/b2-ch07 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch07 --parallel 2
ctest --test-dir build/b2-ch07 --output-on-failure

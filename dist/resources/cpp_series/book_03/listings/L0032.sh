cmake -S source/book3/ch07 -B build/b3-ch07 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch07 --parallel 2
ctest --test-dir build/b3-ch07 --output-on-failure

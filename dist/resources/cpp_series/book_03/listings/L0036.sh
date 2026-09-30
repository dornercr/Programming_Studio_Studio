cmake -S source/book3/ch08 -B build/b3-ch08 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch08 --parallel 2
ctest --test-dir build/b3-ch08 --output-on-failure

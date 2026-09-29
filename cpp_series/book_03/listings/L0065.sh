cmake -S source/book3/ch15 -B build/b3-ch15 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch15 --parallel 2
ctest --test-dir build/b3-ch15 --output-on-failure

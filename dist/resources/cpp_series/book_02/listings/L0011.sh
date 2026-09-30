cmake -S source/book2/ch02 -B build/b2-ch02 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch02 --parallel 2
ctest --test-dir build/b2-ch02 --output-on-failure

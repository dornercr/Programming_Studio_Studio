cmake -S source/book3/ch02 -B build/b3-ch02 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch02 --parallel 2
ctest --test-dir build/b3-ch02 --output-on-failure

cmake -S source/book3/ch05 -B build/b3-ch05 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch05 --parallel 2
ctest --test-dir build/b3-ch05 --output-on-failure

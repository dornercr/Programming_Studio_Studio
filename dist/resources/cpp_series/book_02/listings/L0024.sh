cmake -S source/book2/ch05 -B build/b2-ch05 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch05 --parallel 2
ctest --test-dir build/b2-ch05 --output-on-failure

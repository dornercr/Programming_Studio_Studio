cmake -S source/book2/ch06 -B build/b2-ch06 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch06 --parallel 2
ctest --test-dir build/b2-ch06 --output-on-failure

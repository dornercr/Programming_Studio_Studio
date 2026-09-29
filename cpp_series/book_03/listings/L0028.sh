cmake -S source/book3/ch06 -B build/b3-ch06 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch06 --parallel 2
ctest --test-dir build/b3-ch06 --output-on-failure

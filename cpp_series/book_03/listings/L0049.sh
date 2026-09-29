cmake -S source/book3/ch11 -B build/b3-ch11 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch11 --parallel 2
ctest --test-dir build/b3-ch11 --output-on-failure

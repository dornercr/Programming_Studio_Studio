cmake -S source/book2/ch11 -B build/b2-ch11 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch11 --parallel 2
ctest --test-dir build/b2-ch11 --output-on-failure

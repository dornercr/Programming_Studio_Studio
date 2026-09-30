cmake -S source/book2/ch17 -B build/b2-ch17 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch17 --parallel 2
ctest --test-dir build/b2-ch17 --output-on-failure

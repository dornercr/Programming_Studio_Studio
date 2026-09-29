cmake -S source/book3/ch17 -B build/b3-ch17 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch17 --parallel 2
ctest --test-dir build/b3-ch17 --output-on-failure

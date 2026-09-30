cmake -S source/book2/ch14 -B build/b2-ch14 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch14 --parallel 2
ctest --test-dir build/b2-ch14 --output-on-failure

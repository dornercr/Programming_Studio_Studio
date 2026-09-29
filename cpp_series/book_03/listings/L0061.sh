cmake -S source/book3/ch14 -B build/b3-ch14 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch14 --parallel 2
ctest --test-dir build/b3-ch14 --output-on-failure

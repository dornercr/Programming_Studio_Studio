cmake -S source/book2/ch20 -B build/b2-ch20 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch20 --parallel 2
ctest --test-dir build/b2-ch20 --output-on-failure

cmake -S source/book3/ch20 -B build/b3-ch20 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch20 --parallel 2
ctest --test-dir build/b3-ch20 --output-on-failure

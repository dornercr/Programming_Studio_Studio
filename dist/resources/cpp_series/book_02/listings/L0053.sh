cmake -S source/book2/ch12 -B build/b2-ch12 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch12 --parallel 2
ctest --test-dir build/b2-ch12 --output-on-failure

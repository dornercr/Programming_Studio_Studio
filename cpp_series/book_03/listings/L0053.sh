cmake -S source/book3/ch12 -B build/b3-ch12 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch12 --parallel 2
ctest --test-dir build/b3-ch12 --output-on-failure

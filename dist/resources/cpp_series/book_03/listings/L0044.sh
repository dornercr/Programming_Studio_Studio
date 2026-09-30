cmake -S source/book3/ch10 -B build/b3-ch10 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch10 --parallel 2
ctest --test-dir build/b3-ch10 --output-on-failure

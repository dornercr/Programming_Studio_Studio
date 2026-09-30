cmake -S source/book2/ch10 -B build/b2-ch10 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch10 --parallel 2
ctest --test-dir build/b2-ch10 --output-on-failure

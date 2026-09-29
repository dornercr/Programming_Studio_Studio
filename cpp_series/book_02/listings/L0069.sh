cmake -S source/book2/ch16 -B build/b2-ch16 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch16 --parallel 2
ctest --test-dir build/b2-ch16 --output-on-failure

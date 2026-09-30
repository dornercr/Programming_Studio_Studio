cmake -S source/book3/ch16 -B build/b3-ch16 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch16 --parallel 2
ctest --test-dir build/b3-ch16 --output-on-failure

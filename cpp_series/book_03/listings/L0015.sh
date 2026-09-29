cmake -S source/book3/ch03 -B build/b3-ch03 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch03 --parallel 2
ctest --test-dir build/b3-ch03 --output-on-failure

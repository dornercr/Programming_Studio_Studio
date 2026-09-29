cmake -S source/book2/ch03 -B build/b2-ch03 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch03 --parallel 2
ctest --test-dir build/b2-ch03 --output-on-failure

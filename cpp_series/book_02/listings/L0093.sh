cmake -S source/book2/ch21 -B build/b2-ch21 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch21 --parallel 2
ctest --test-dir build/b2-ch21 --output-on-failure

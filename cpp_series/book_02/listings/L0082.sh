cmake -S source/book2/ch19 -B build/b2-ch19 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b2-ch19 --parallel 2
ctest --test-dir build/b2-ch19 --output-on-failure

cmake -S source/book3/ch19 -B build/b3-ch19 \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build/b3-ch19 --parallel 2
ctest --test-dir build/b3-ch19 --output-on-failure

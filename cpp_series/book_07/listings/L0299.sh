cmake -S source -B build/lab -DCMAKE_BUILD_TYPE=Debug
cmake --build build/lab --parallel 4
ctest --test-dir build/lab --output-on-failure

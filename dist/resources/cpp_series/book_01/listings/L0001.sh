cmake -S source -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 4
ctest --test-dir build --parallel 4 --output-on-failure

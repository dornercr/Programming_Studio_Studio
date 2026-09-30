cmake --version
c++ --version
python3 --version
cmake -S source -B build/operations -DCMAKE_BUILD_TYPE=Debug
cmake --build build/operations --parallel 4
ctest --test-dir build/operations --output-on-failure

cmake -S source -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target harbor_relay harbor_relay_tests -j 2
./build/bin/harbor_relay --self-test
ctest --test-dir build --output-on-failure -R '^relay_'

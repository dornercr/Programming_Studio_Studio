cmake -S source -B build/routes -DCMAKE_BUILD_TYPE=Debug
cmake --build build/routes --target harbor_routes harbor_routes_tests --parallel 2
ctest --test-dir build/routes -R '^harbor_routes_regression$' --output-on-failure
printf '4 4 0 3\n0 1 8\n0 2 1\n2 1 1\n1 3 1\n' |
  build/routes/supplemental/harbor_routes

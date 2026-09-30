set -eu
cmake -S . -B build >configure.log
cmake --build build >build.log
./build/app
cmake -S . -B build -DCONFIG_GENERATION=8 >>configure.log
cmake --build build >>build.log
./build/app

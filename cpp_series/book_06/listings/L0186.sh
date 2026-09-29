set -eu
: "${CUDA_ARCH:?Set a CUDA architecture supported by your toolkit and deployment GPU}"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CUDA_ARCHITECTURES="$CUDA_ARCH"
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build --prefix "$PWD/stage"
sha256sum stage/bin/harbor_gpu

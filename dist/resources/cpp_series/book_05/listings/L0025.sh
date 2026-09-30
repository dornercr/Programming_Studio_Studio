cmake --version
g++ --version | head -1
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --verbose > build-command.log 2>&1
sha256sum build/app > artifact.sha256

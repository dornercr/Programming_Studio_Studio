mkdir -p project/{src,include,cmake}
cmake -S project -B project/build -DCMAKE_BUILD_TYPE=Debug
cmake --build project/build
find project/build -maxdepth 2 -type f | head

set -eu
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug >configure.log
cmake --build build --parallel 3 >build.log
python3 lab.py

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
clang-tidy src/parser.cpp -p build \
  --checks='bugprone-*,performance-*,readability-*'

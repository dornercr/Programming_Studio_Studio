set -eu
g++ -std=c++20 -O2 main.cpp -o app
test "$(./app)" = ready-v1
mkdir -p stage/bin
cp app stage/bin/app
cmp app stage/bin/app
tar -czf release.tar.gz -C stage .
mkdir unpack
tar -xzf release.tar.gz -C unpack
cmp app unpack/bin/app
test "$(unpack/bin/app)" = ready-v1
printf 'tested, packaged, extracted: identical binary runs\n'

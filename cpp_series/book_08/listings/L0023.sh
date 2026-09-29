set -eu
g++ -std=c++20 main.cpp -o app
test "$(./app check)" = healthy
if ./app wrong >out.txt 2>err.txt; then exit 1; else status=$?; fi
test "$status" = 2
grep -qx 'invalid mode' err.txt
printf 'success output and rejection exit2 verified\n'

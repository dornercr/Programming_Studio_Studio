set -eu
g++ -std=c++20 main.cpp -o app
ldd ./app >dependencies.txt
if grep -q 'not found' dependencies.txt; then cat dependencies.txt >&2; exit 1; fi
./app
readelf -l app >program-headers.txt
grep -q INTERP program-headers.txt
printf 'loader path recorded; host dependencies resolved\n'

set -euo pipefail
output=$(./app --input fixtures/valid.txt)
status=$?
printf 'status=%s output=%s\n' "$status" "$output"
test "$status" -eq 0
test "$output" = "records=3"

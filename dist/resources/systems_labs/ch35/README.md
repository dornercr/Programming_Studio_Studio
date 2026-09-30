# Flush the final partial buffer

Combine characters into batches of four, then forward any remaining suffix at the end. Do not count an empty final flush as a data transfer.

## Acceptance checks

- ABCDE produces two forwarded batches and the complete text.
- ABCD produces exactly one batch.
- Empty input produces no batch.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch35/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A partial final buffer still contains real data. The explicit final flush closes that gap, while the empty check prevents a misleading transfer count. This model counts forwarding to another memory string. Library flush, kernel acceptance, and durable storage are different boundaries in a real output path.

## Bug to diagnose

Flush only when pending.size()==4. The final E never reaches output.

Flush a nonempty suffix when the operation finishes. Test a size not divisible by the buffer capacity.

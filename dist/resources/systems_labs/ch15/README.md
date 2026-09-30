# Guard signed division boundaries

Return quotient and remainder for int inputs. Reject division by zero and the minimum int divided by -1, whose quotient cannot fit in int.

## Acceptance checks

- -17/5 returns quotient -3 and remainder -2.
- 17/-5 returns -3 and 2.
- Zero divisor and the unrepresentable minimum/-1 case are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch15/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The guards run before either / or %, since both need a valid divisor and representable quotient. optional marks rejection separately from a legitimate zero quotient. For valid cases, quotient*divisor+remainder equals the dividend; the remainder follows the dividend’s sign unless it is zero.

## Bug to diagnose

Check only divisor != 0. The minimum-int/-1 case is still not representable.

Check the exceptional signed boundary as well as zero. Perform the checks before evaluating either arithmetic expression.

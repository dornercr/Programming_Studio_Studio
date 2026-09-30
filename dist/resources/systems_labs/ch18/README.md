# Test an alias-sensitive rewrite

Implement write_then_read(a,b) and verify both separate objects and two references to the same object. Do not move the read before the write.

## Acceptance checks

- Separate a=4,b=6 returns 6 and changes a to 9.
- When a and b alias x=4, the return is 9.
- A cached-before-write variant returns 4 for the aliasing case and is not equivalent.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch18/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A const reference prevents mutation through that reference; it does not prove the underlying object cannot change through another name. The write through a can therefore affect the later read through b. Optimization must preserve this allowed input case unless an additional valid non-aliasing contract excludes it.

## Bug to diagnose

Cache b before assigning a and assume distinct parameter names mean distinct objects.

Preserve the order or supply and enforce a stronger non-overlap contract. Test both aliasing and non-aliasing arguments.

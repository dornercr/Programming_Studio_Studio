# Make page permissions part of a lookup

Represent a page entry with present, readable, and writable flags. A read and write request must use the corresponding permission, and an absent page denies both.

## Acceptance checks

- A present read-only entry accepts reads and rejects writes.
- A present read-write entry accepts both.
- An absent entry rejects both regardless of permission bits.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch24/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The entry groups flags that describe one mapping. Presence and permission answer different questions, so neither can replace the other. The pure allows function is easy to test and has no state changes. Architecture-specific user/supervisor and execute permissions are outside this limited model and would need explicit fields and checks.

## Bug to diagnose

Return entry.present for every access. A present read-only mapping then accepts a write.

Pass the requested access type and check its matching permission after presence. Test the same entry with two different operations.

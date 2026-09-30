# Make copying and borrowing visible

Write one function that returns a changed copy and one that changes the caller’s vector. Reject an empty vector before accessing its first element.

## Acceptance checks

- copy_changed leaves the caller unchanged.
- change_in_place updates the caller.
- Both functions reject an empty vector.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch02/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A value parameter creates a separate vector object. A reference parameter, marked &, names the existing vector. The empty check is a precondition check performed before indexing; throwing leaves the original vector unchanged. Return-by-value lets the returned vector own its storage without a dangling reference.

## Bug to diagnose

Remove & from change_in_place. The function changes only its local copy, and the caller still sees 2.

Use a reference for the in-place operation and a value parameter for the copy operation. Name the operations so a caller can see the difference.

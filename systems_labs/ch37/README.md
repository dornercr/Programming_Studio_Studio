# Separate a descriptor handle from its open-file state

Model two inherited handles referring to one open-file description. Closing the child’s handle must not close the parent’s handle or reset their shared offset.

## Acceptance checks

- A child read advances the parent-visible offset.
- Resetting the child handle leaves the parent usable.
- A separately opened description has an independent offset.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch37/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The handle is not the open-file description. Each process can close its own inherited descriptor while the kernel object remains referenced by another descriptor. The description holds the shared offset in this model. These smart pointers exist in one C++ process and illustrate the relationship; they are not the implementation of fork.

## Bug to diagnose

Give the child a fresh OpenFile copy and assume its offset is shared just because its initial value matches.

Represent shared open-file state with one backing object. Keep independent opens distinct from duplicated or inherited references.

# Return an owned mapping snapshot

Contrast an independent snapshot with a shared reference to backing bytes. Then destroy the original handle and verify a copied owner still keeps the backing alive.

## Acceptance checks

- Private snapshot edits do not alter shared backing.
- Shared edits are visible through both owners.
- Resetting one owner does not destroy backing still owned by another.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch36/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Shared ownership and shared data are related but different facts. A shared_ptr keeps this object alive; a separate string snapshot copies its characters. A real memory mapping is governed by OS mapping and file-lifetime rules rather than shared_ptr counts. The model makes the intended aliasing and cleanup relationships inspectable without pretending to implement mmap.

## Bug to diagnose

Store only a raw pointer from backing.get(), reset the last owner, then dereference that pointer.

Retain ownership for as long as the object must live, or ensure a borrowed view ends before its owner. A borrowed address cannot keep backing alive.

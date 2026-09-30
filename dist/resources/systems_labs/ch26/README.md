# Detach a shared value before a private write

Implement a single-threaded copy-on-write update for shared_ptr<int>. Reject a null handle, copy only when another owner shares the object, and preserve the other owner’s value.

## Acceptance checks

- The first private update detaches a shared object.
- Updating an already-unique object keeps the same address.
- A null handle is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch26/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Copying the old value before assignment preserves the private-write contract. If allocation throws, the assignment to owner has not yet committed and the original shared value remains intact. unique() is suitable only under this lab’s single-threaded access rule; it does not establish a safe concurrent mutation protocol. OS copy-on-write applies similar intent at a different boundary.

## Bug to diagnose

Write through child before making the private copy. The parent observes the mutation before separation occurs.

Prepare private backing first, then write it. Keep the thread-safety and lifetime assumptions explicit.

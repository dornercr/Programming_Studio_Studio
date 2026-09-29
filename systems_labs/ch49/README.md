# Build a bounded, drainable queue

Implement a capacity-two queue with blocking push and pop. close rejects new pushes, wakes waiting participants, and permits consumers to drain existing items before reporting end.

## Acceptance checks

- Values 1 through 5 are delivered in FIFO order.
- The consumer drains all five after producer closure.
- A closed empty queue returns no value; a push after close is rejected.
- Zero capacity is rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch49/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

A class is useful here because the mutex, predicates, queue, capacity, and closed flag must obey one shared rule across several operations. All shared state is accessed under the mutex. Separate conditions describe available items and available space. The producer closes on normal completion and on exceptions so the consumer cannot wait forever for more input. The queue must outlive every participant.

## Bug to diagnose

In pop, return no value whenever closed_ is true, even if items_ is nonempty. Buffered work is lost during shutdown.

After waiting, return no value only when the queue is empty. A closed queue can still contain items that the drain contract promises to deliver.

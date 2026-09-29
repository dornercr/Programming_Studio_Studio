# Use a condition variable with a lasting predicate

Implement a one-shot handoff using a mutex, condition variable, ready flag, and payload. It must work whether the producer sets ready before or after the consumer starts waiting.

## Acceptance checks

- The consumer sees payload 42.
- The wait uses a predicate while holding the associated mutex.
- The producer changes the state under that same mutex.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch48/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The predicate is durable state; a notification is only a reason to recheck it. wait releases the lock while waiting and reacquires it before evaluating readiness and returning. If ready was already true, the consumer does not sleep. The final get ensures the notification call and producer lifetime finish before local synchronization objects are destroyed.

## Bug to diagnose

Call changed.wait(lock) once without a predicate, then read payload. A spurious wakeup or an earlier notification can break the intended protocol.

Use the predicate overload or an explicit while loop. Keep state reads and writes under the same mutex.

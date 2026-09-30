# Return an owner across a function boundary

Create a unique owner in a factory function, return it, move it to a second owner, and show the original owner is empty afterward.

## Acceptance checks

- make_value(42) returns a non-null owner.
- The value survives the factory’s local scope.
- After a move, only the destination owner retains the object.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch08/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The allocated int and the local unique_ptr are different objects. Returning the owner transfers responsibility without returning the address of a dying local int. std::move allows transfer; the unique_ptr operation performs it. The second owner releases the int when its scope ends, including during stack unwinding.

## Bug to diagnose

Return the address of a local int from the factory. It points at an object whose lifetime ends when the function returns.

Return the value itself when ownership is unnecessary, or return an owning handle for a separately allocated object. A raw address cannot extend lifetime.

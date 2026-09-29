# Model nonblocking drain outcomes

Consume a supplied trace of read results: positive counts add bytes, -1 means would-block in this model, and zero means EOF. Reject other negative values.

## Acceptance checks

- 2,1,-1 buffers three bytes and reports blocked, not EOF.
- 2,0 buffers two bytes and reports EOF.
- Results after would-block or EOF are not consumed in the same drain call.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch53/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Would-block means the operation cannot make immediate progress; EOF means the stream has ended. Keeping them separate prevents a temporary lack of data from closing a connection. The model gives -1 one special meaning, while a real read returning -1 requires inspecting errno. The fixture uses small counts; production accumulation also needs an explicit buffer-size bound.

## Bug to diagnose

Treat every nonpositive result as EOF. A live connection is closed when it merely has no bytes ready now.

Distinguish success, EOF, would-block, interruption, and permanent errors according to the API. Return control to the event loop on would-block.

# Reject an incomplete pipeline before launching it

Parse a deliberately small grammar: word, pipe, word, separated by spaces. Reject missing stages, extra words, and multiple pipes. This parser has no quotes, expansions, or redirection.

## Acceptance checks

- emit | count returns the two stage names.
- emit | and | count are rejected.
- emit | count extra and a | b | c are rejected.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch42/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Parsing produces an execution plan before any process is launched. The result owns its stage names, so it survives destruction of the input stream. Rejecting unsupported forms makes the small grammar explicit; it would be incorrect to advertise this whitespace parser as a general shell parser.

## Bug to diagnose

Launch the first stage as soon as the first word is read. Later syntax failure then occurs after side effects have already begun.

Separate parsing from execution. Validate the complete structure and only then allocate pipes or launch children.

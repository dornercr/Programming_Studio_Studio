From the project root:

```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch19/main.cpp coding_lab/book_one_expansion/ch19/billing.cpp -o /tmp/billing
printf '7 4\n' | /tmp/billing
```
Expected output: `28` followed by a newline. The header introduces the contract; billing.cpp defines it; main.cpp calls it. Omitting billing.cpp produces an undefined-reference link error.

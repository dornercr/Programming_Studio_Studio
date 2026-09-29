# Count TLB walks separately from data hits

Model a translation cache over a fixed page table. The first lookup of a mapped page performs a walk; the second reuses the translation. An unmapped page must not enter the TLB.

## Acceptance checks

- Two lookups of page 1 produce one successful walk.
- Page 2 is unmapped and is not cached.
- Clearing the TLB forces another walk for page 1.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch25/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

The two maps represent different levels of information: the page table is the model’s mapping authority, and the TLB is a derived cache. Invalidation is necessary when an authoritative mapping changes. Walk count includes the failed unmapped lookup, while data-cache behavior is intentionally not modeled by this function.

## Bug to diagnose

Update the page table while continuing to trust an old TLB entry. The next hit returns stale frame information.

Invalidate or update cached translations under the platform’s rules when mappings change. This lab uses clear to make that dependency observable.

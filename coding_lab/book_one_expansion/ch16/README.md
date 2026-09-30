# Chapter 16: Validate a record before a state transition

Define enum class ParcelState { pending, ready } and struct Parcel { int weight; ParcelState state; }. Permit pending to ready only for weight 1 through 100. Every rejection leaves all fields unchanged.

## Design and rules
A named scoped state type exposes legal states. Validation precedes the one allowed field change.

Only pending parcels with allowed weight become ready; rejected records retain all prior field values.

Alternative: A class can enforce this rule at every mutation boundary. A public struct is sufficient when callers follow this deliberately narrow operation contract.

Starter defect: The starter rejects weight zero but still changes pending to ready. A false return does not undo the assignment.

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch16/solution.cpp -o /tmp/book-one-ch16
printf '%s' '' | /tmp/book-one-ch16
```
Expected standard output:
```text
false true
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.

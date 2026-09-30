# Keep failed replacement from destroying the current image

Model exec as a prepared replacement. Validate the new program name before committing it; preserve process identity on success and the old image on failure.

## Acceptance checks

- Replacing shell with worker keeps pid 42.
- An empty name is rejected without changing the image.
- Success and failure are distinct results.

## Build

From the source ZIP root:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread systems_labs/ch38/solution.cpp -o lab
./lab
```

Substitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.

## Explained solution

Preparation can fail before the new state becomes visible. Swapping commits the prepared string while preserving the model’s process identifier. A real successful exec does not return into the old program; this returning function is only a state model. In actual code, the path after exec is the failure path.

## Bug to diagnose

Clear the current image before validating the replacement. An invalid request then destroys valid existing state.

Validate and prepare first, then commit. In real exec code, keep failure reporting on the path that returns from the call.

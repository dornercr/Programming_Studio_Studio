git status --short
git diff -- src/parser.cpp tests/parser_test.cpp
git add src/parser.cpp tests/parser_test.cpp
git diff --cached
git commit -m "Reject truncated parser frames"

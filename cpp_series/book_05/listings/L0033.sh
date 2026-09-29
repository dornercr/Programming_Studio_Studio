git switch -c feature/parser-limit
# edit and commit the feature
git switch main
git merge feature/parser-limit
# after resolving conflicts:
git add src/parser.cpp
git diff --cached
git commit

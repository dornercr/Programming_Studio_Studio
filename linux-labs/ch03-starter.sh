#!/usr/bin/env bash
set -euo pipefail
# Distinguish names from open file contents
# Use a disposable directory and satisfy the contract below.
# A hard link retains the inode; a symbolic link still targets the removed name.
# Expected stdout: hard link retained; symbolic path broken
# TODO: implement the observation and its assertion.
exit 1

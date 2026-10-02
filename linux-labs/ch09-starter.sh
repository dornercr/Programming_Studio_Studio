#!/usr/bin/env bash
set -euo pipefail
# Detect inode exhaustion independently of bytes
# Use a disposable directory and satisfy the contract below.
# Use a controlled capacity fixture rather than filling the real filesystem.
# Expected stdout: bytes available; inode boundary exhausted
# TODO: implement the observation and its assertion.
exit 1

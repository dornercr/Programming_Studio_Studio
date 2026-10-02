#!/usr/bin/env bash
set -euo pipefail
# Detect a configured resource ceiling
# Use a disposable directory and satisfy the contract below.
# Integer fixtures represent a resource contract. They do not measure or configure real cgroups.
# Expected stdout: fixture demand exceeds configured ceiling
# TODO: implement the observation and its assertion.
exit 1

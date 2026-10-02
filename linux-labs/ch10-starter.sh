#!/usr/bin/env bash
set -euo pipefail
# Check both volume and filesystem growth
# Use a disposable directory and satisfy the contract below.
# This arithmetic model represents two layers; it does not resize real storage.
# Expected stdout: LV grew to 140; filesystem remains 100
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash
set -euo pipefail
# Detect drift in an installed-file fixture
# Use a disposable directory and satisfy the contract below.
# A manifest detects changed bytes; it does not attest that a package repository or service is healthy.
# Expected stdout: original verified; drift detected
# TODO: implement the observation and its assertion.
exit 1

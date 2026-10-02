#!/usr/bin/env bash
set -euo pipefail
# Verify private local content mode
# Use a disposable directory and satisfy the contract below.
# Inspect actual modes on a temporary tree; mode evidence is separate from ACL or mandatory policy.
# Expected stdout: directory=700; file=600
# TODO: implement the observation and its assertion.
exit 1

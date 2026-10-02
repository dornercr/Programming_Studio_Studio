#!/usr/bin/env bash
set -euo pipefail
# Find a missing cloud rebuild dependency
# Use a disposable directory and satisfy the contract below.
# A local record models cloud rebuild requirements without using an account or cloud API.
# Expected stdout: instance image available; role dependency absent
# TODO: implement the observation and its assertion.
exit 1

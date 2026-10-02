#!/usr/bin/env bash
set -euo pipefail
# Distinguish connectivity from listening service
# Use a disposable directory and satisfy the contract below.
# Layered fixture evidence prevents a route test from becoming an application-health claim.
# Expected stdout: route evidence present; listener still missing
# TODO: implement the observation and its assertion.
exit 1

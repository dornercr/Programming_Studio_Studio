#!/usr/bin/env bash
set -euo pipefail
# Expose a scheduled job environment dependency
# Use a disposable directory and satisfy the contract below.
# Validate a local script and its explicit environment without registering a cron job or timer.
# Expected stdout: missing environment rejected; explicit environment accepted
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash
set -euo pipefail
# Preserve data outside an ephemeral runtime directory
# Use a disposable directory and satisfy the contract below.
# Temporary directories model the storage distinction; this does not attest container isolation.
# Expected stdout: runtime removed; durable fixture retained
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash
set -euo pipefail
# Find a cross-host numeric-identity mismatch
# Use a disposable directory and satisfy the contract below.
# Use numeric identity fixtures; no NFS or Samba service is installed.
# Expected stdout: same display name; numeric identity differs
# TODO: implement the observation and its assertion.
exit 1

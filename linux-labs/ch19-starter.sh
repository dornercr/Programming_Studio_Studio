#!/usr/bin/env bash
set -euo pipefail
# Verify a release marker rather than any page
# Use a disposable directory and satisfy the contract below.
# Read a local response fixture. Real release acceptance also checks endpoint, status and trusted TLS.
# Expected stdout: approved marker present; wrong release rejected
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash
set -euo pipefail
# Reject a changed recorded host identity
# Use a disposable directory and satisfy the contract below.
# Compare invented identity records. Real SSH uses its host-key verification tools, not this simplified text record.
# Expected stdout: identity mismatch; investigate before trust
# TODO: implement the observation and its assertion.
exit 1

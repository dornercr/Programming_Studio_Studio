#!/usr/bin/env bash
set -euo pipefail
# Apply a rollback trigger from an acceptance contract
# Use a disposable directory and satisfy the contract below.
# A release-record model exercises an explicit rollback decision; it does not deploy a real service.
# Expected stdout: acceptance failed; rollback selected: release-7
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash
set -euo pipefail
# Check the dependencies of a VM recovery record
# Use a disposable directory and satisfy the contract below.
# The model checks a recovery record. It does not create a hypervisor snapshot.
# Expected stdout: disk exists; network dependency still missing
# TODO: implement the observation and its assertion.
exit 1

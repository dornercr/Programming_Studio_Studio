#!/usr/bin/env bash
set -euo pipefail
# Converge without repeated changes
# Use a disposable directory and satisfy the contract below.
# Exit 10 is this fixture's change indicator, not an Ansible exit-code convention.
# Expected stdout: first run changed; second run unchanged
# TODO: implement the observation and its assertion.
exit 1

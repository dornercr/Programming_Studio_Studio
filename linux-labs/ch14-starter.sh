#!/usr/bin/env bash
set -euo pipefail
# Keep the boot identity with selected log evidence
# Use a disposable directory and satisfy the contract below.
# Select the current boot and exact unit; these are invented records, not journalctl output.
# Expected stdout: boot=B unit=web result=failed
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash
set -euo pipefail
# Separate discretionary and type-enforcement decisions
# Use a disposable directory and satisfy the contract below.
# A type-policy model explains an independent boundary. It neither invokes nor tests a real SELinux policy.
# Expected stdout: DAC permits; type mismatch remains
# TODO: implement the observation and its assertion.
exit 1

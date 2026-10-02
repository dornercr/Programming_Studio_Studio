#!/usr/bin/env bash
set -euo pipefail
# Keep start state distinct from boot enablement
# Use a disposable directory and satisfy the contract below.
# Interpret fixture properties; this exercise does not operate a real systemd unit.
# Expected stdout: running now; not enabled for boot
# TODO: implement the observation and its assertion.
exit 1

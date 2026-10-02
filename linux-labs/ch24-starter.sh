#!/usr/bin/env bash
set -euo pipefail
# Keep installed and running kernel versions separate
# Use a disposable directory and satisfy the contract below.
# A fixture distinguishes package state from runtime; it does not install a kernel or reboot.
# Expected stdout: installed and running versions differ
# TODO: implement the observation and its assertion.
exit 1

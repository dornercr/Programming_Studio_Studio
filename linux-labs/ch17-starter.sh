#!/usr/bin/env bash
set -euo pipefail
# Reject a credential fixture with broad permissions
# Use a disposable directory and satisfy the contract below.
# The fixture contains no real credential. Production security still requires identity, path and policy review.
# Expected stdout: broad mode detected; private mode verified
# TODO: implement the observation and its assertion.
exit 1

#!/usr/bin/env bash

# Fail on error
set -e

# Set ${SCRIPT_DIR}
SCRIPT_DIR="$(dirname -- "$(readlink -f -- "${0}")")"

# Activate venv
. "${SCRIPT_DIR}"/.venv/bin/activate

# Install dependencies
pip install notebook numpy scipy matplotlib

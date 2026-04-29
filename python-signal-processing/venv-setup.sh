#!/usr/bin/env bash

# Fail on error
set -e

# Set ${SCRIPT_DIR}
SCRIPT_DIR="$(dirname -- "$(readlink -f -- "${0}")")"

# Create venv
python -m venv ${SCRIPT_DIR}/.venv

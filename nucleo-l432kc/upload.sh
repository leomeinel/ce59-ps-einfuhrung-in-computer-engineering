#!/usr/bin/env bash

# Fail on error
set -e

# Upload to device
pio run -t upload

#!/usr/bin/env bash
# shellcheck shell=bash

set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# shellcheck source=config.sh
source "${SCRIPT_DIR}/config.sh"

###############################################################################
# Toolchain
###############################################################################

TOOLCHAIN_URL="https://mirrors.edge.kernel.org/pub/tools/crosstool/files/bin/x86_64/14.2.0/x86_64-gcc-14.2.0-nolibc-aarch64-linux.tar.xz"
TOOLCHAIN_SHA256="53deb48c660f1453ca79fa224d44de1145c8c5377d90040934af063d1b726b29"

if [[ -x "${TOOLCHAIN_BIN}/aarch64-linux-gcc" ]]; then
    msg "Toolchain already present at ${TOOLCHAIN_DIR}"
    exit 0
fi

msg "Fetching aarch64 cross-compiler toolchain (gcc 14.2.0, nolibc)"

TMP_ARCHIVE="$(mktemp -t toolchain-XXXXXX.tar.xz)"
trap 'rm -f "$TMP_ARCHIVE"' EXIT

curl \
    --fail \
    --location \
    --silent \
    --show-error \
    --retry 5 \
    --retry-delay 5 \
    --retry-all-errors \
    -o "$TMP_ARCHIVE" \
    "$TOOLCHAIN_URL"

msg "Verifying checksum"
echo "${TOOLCHAIN_SHA256}  ${TMP_ARCHIVE}" | sha256sum -c -

mkdir -p "${REPO_ROOT}/toolchains"
tar -xJf "$TMP_ARCHIVE" -C "${REPO_ROOT}/toolchains"

msg "Toolchain installed at ${TOOLCHAIN_DIR}"

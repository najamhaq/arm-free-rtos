#!/usr/bin/env bash
set -euo pipefail

command -v git >/dev/null 2>&1 || { echo "ERROR: git not found"; exit 1; }
command -v clang-format >/dev/null 2>&1 || { echo "ERROR: clang-format not found"; exit 1; }

FILES=$(git ls-files '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hpp' || true)
[ -z "$FILES" ] && exit 0

clang-format --dry-run --Werror $FILES
echo "OK: formatting clean."

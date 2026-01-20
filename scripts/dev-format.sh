#!/usr/bin/env bash
set -euo pipefail

command -v git >/dev/null 2>&1 || { echo "ERROR: git not found"; exit 1; }
command -v clang-format >/dev/null 2>&1 || { echo "ERROR: clang-format not found"; exit 1; }

FILES=$(git ls-files -z '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hpp' || true)
[ -z "$FILES" ] && { echo "No C/C++ files found."; exit 0; }

echo "Formatting source files..."
echo "$FILES" | xargs -0 -n 50 clang-format -i
echo "OK: formatting complete."

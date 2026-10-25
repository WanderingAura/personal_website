#!/usr/bin/env bash
set -euo pipefail

# the root CMakeLists.txt reads $EMSDK for the Emscripten sysroot
if [ -z "${EMSDK:-}" ]; then
  echo "EMSDK is not set — source emsdk_env.sh first" >&2
  exit 1
fi

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT/build-web"

mkdir -p "$BUILD_DIR"
cmake -S "$ROOT" -B "$BUILD_DIR" \
  -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
  -DPLATFORM=Web -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD_DIR"

DEST="$ROOT/site/wasm"
mkdir -p "$DEST"
cp "$BUILD_DIR/src/personal_gallery.js" "$DEST/"
cp "$BUILD_DIR/src/personal_gallery.wasm" "$DEST/"
cp "$BUILD_DIR/src/personal_gallery.data" "$DEST/"

echo "WASM artifacts copied to $DEST"

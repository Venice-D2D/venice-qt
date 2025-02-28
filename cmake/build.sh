#!/bin/bash

PROJECT_DIR=$(realpath "../")
BUILD_DIR="$PROJECT_DIR/build"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

cmake ..
make -j$(nproc)

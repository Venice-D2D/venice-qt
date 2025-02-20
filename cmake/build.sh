#!/bin/bash

PROJECT_DIR=$(realpath "../")
BUILD_DIR="$PROJECT_DIR/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

make clean
cmake ..
make -j$(nproc)

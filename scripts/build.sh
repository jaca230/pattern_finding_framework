#!/bin/bash

# Get the absolute path of the script directory
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BASE_DIR="$SCRIPT_DIR/.."
BUILD_DIR="$BASE_DIR/build"

# Default overwrite flag is false
OVERWRITE=false

# Help message
show_help() {
    echo "Usage: ./build.sh [OPTIONS]"
    echo
    echo "Options:"
    echo "  -o, --overwrite   Remove existing build directory before building"
    echo "  -h, --help        Display this help message"
}

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -o|--overwrite) OVERWRITE=true; shift ;;
        -h|--help) show_help; exit 0 ;;
        *) echo "[ERROR] Unknown option: $1"; show_help; exit 1 ;;
    esac
done

# If overwrite flag is set, remove the build directory
if [ "$OVERWRITE" = true ]; then
    echo "[INFO] Overwrite enabled: Cleaning previous build..."
    rm -rf "$BUILD_DIR"
fi

# Create the build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Run CMake and Make
echo "[INFO] Configuring the project with CMake..."
cmake "$BASE_DIR"

echo "[INFO] Building the project..."
make

echo "[INFO] Build complete. Executables are in: $BUILD_DIR/bin/"

#!/bin/bash

# Get the absolute path of the script directory
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BASE_DIR="$SCRIPT_DIR/.."
BUILD_DIR="$BASE_DIR/build"

# Default flags
OVERWRITE=false
NUM_PROC=$(nproc)  # Default: use all available processors

# Help message
show_help() {
    echo "Usage: ./build.sh [OPTIONS]"
    echo
    echo "Options:"
    echo "  -o, --overwrite        Remove existing build directory before building"
    echo "  -j, --jobs <number>     Specify number of processors to use (default: all available)"
    echo "  -h, --help              Display this help message"
}

# Parse arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -o|--overwrite) OVERWRITE=true; shift ;;
        -j|--jobs)
            if [[ -n "$2" && "$2" != -* ]]; then
                NUM_PROC=$2
                shift 2
            else
                echo "[build.sh, ERROR] Missing number of jobs after -j/--jobs"
                exit 1
            fi
            ;;
        -h|--help) show_help; exit 0 ;;
        *) echo "[build.sh, ERROR] Unknown option: $1"; show_help; exit 1 ;;
    esac
done

# If overwrite flag is set, remove the build directory
if [ "$OVERWRITE" = true ]; then
    echo "[build.sh, INFO] Overwrite enabled: Cleaning previous build..."
    rm -rf "$BUILD_DIR"
fi

# Create the build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Run CMake and Make
echo "[build.sh, INFO] Configuring the project with CMake..."
cmake "$BASE_DIR"

echo "[build.sh, INFO] Building the project using $NUM_PROC processor(s)..."
make -j"$NUM_PROC"

echo "[build.sh, INFO] Build complete. Executables are in: $BUILD_DIR/bin/"

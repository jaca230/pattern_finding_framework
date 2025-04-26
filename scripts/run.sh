#!/bin/bash

# Get the absolute path of the script directory
SCRIPT_DIR=$(dirname "$(realpath "$0")")
BASE_DIR="$SCRIPT_DIR/.."
EXECUTABLE="$BASE_DIR/build/bin/pattern_finder"

# Help message
show_help() {
    echo "Usage: ./run.sh [OPTIONS]"
    echo
    echo "Options:"
    echo "  -h, --help        Display this help message"
    echo "  -d, --debug       Run with gdb for debugging"
    echo "  -v, --valgrind    Run with valgrind for memory analysis"
}

# Parse arguments
DEBUG=false
VALGRIND=false

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help) show_help; exit 0 ;;
        -d|--debug) DEBUG=true; shift ;;
        -v|--valgrind) VALGRIND=true; shift ;;
        *) echo "[run.sh, ERROR] Unknown option: $1"; show_help; exit 1 ;;
    esac
done

# Run the executable if it exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "[run.sh, ERROR] Executable not found at: $EXECUTABLE"
    echo "Try running './build.sh' first."
    exit 1
fi

# Debugging or Valgrind execution
if [ "$DEBUG" = true ]; then
    echo "[run.sh, INFO] Running with gdb for debugging..."
    gdb --args "$EXECUTABLE"
elif [ "$VALGRIND" = true ]; then
    echo "[run.sh, INFO] Running with valgrind for memory analysis..."
    valgrind --leak-check=full --track-origins=yes "$EXECUTABLE"
else
    echo "[run.sh, INFO] Running pattern_finder..."
    "$EXECUTABLE"
fi

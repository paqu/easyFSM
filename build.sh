#!/bin/bash
# build.sh - Convenience build script

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Traffic Light Simulator - CMake Build ===${NC}"

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
CLEAN=false
RUN=false
DEBUG=false
TESTS=false
ASAN=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            DEBUG=true
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --run)
            RUN=true
            shift
            ;;
        --tests)
            TESTS=true
            shift
            ;;
        --asan)
            ASAN=true
            shift
            ;;
        --build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --debug      Build debug version"
            echo "  --clean      Clean build directory first"
            echo "  --run        Run after successful build"
            echo "  --tests      Enable unit tests"
            echo "  --asan       Enable AddressSanitizer"
            echo "  --build-dir  Specify build directory (default: build)"
            echo "  --help       Show this help"
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            exit 1
            ;;
    esac
done

# Clean if requested
if [ "$CLEAN" = true ]; then
    echo -e "${YELLOW}Cleaning build directory...${NC}"
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Prepare CMake arguments
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ "$DEBUG" = true ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DBUILD_DEBUG=ON"
fi

if [ "$TESTS" = true ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DBUILD_TESTS=ON"
fi

if [ "$ASAN" = true ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DENABLE_ASAN=ON"
fi

# Configure
echo -e "${YELLOW}Configuring with CMake...${NC}"
cmake $CMAKE_ARGS ..

# Build
echo -e "${YELLOW}Building...${NC}"
cmake --build . --parallel

echo -e "${GREEN}Build completed successfully!${NC}"

# Run if requested
if [ "$RUN" = true ]; then
    echo -e "${YELLOW}Running simulator...${NC}"
    ./bin/traffic_light_simulator
fi

echo -e "${GREEN}Done!${NC}"

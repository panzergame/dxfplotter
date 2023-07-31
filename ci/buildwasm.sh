#! /bin/bash

set -x
set -e

# building in temporary directory to keep system clean
# use RAM disk if possible (as in: not building on CI system like Travis, and RAM disk is available)
if [ "$CI" == "" ] && [ -d /dev/shm ]; then
    TEMP_BASE=/dev/shm
else
    TEMP_BASE=/tmp
fi

BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" analyse-sonarcloud-XXXXXX)

# make sure to clean up build dir, even if errors occur
cleanup () {
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
}
trap cleanup EXIT

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))
OLD_CWD=$(readlink -f .)

# switch to build dir
pushd "$BUILD_DIR"

QT6_DIR="${REPO_ROOT}/${QT_VERSION}/"
QT6_DIR_DESKTOP="${QT6_DIR}/gcc_64/"
QT6_DIR_WASM="${QT6_DIR}/wasm_32/"

# configure build files with CMake
cmake "$REPO_ROOT" -DCMAKE_BUILD_TYPE=Release -DQt6_DIR=${QT6_DIR_WASM} -DQT_HOST_PATH=${QT6_DIR_DESKTOP}
cmake --build .



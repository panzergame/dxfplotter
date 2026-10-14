#! /bin/bash

set -x
set -e

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))

cd "$REPO_ROOT"

BUILD_DIR=build

cmake -B $BUILD_DIR -DBUILD_TESTING=OFF -DWITH_ORTOOLS=ON -DQt6_DIR=${Qt_ROOT}/lib/cmake/Qt6 -DCMAKE_PREFIX_PATH=${ORTOOLS_ROOT}
cmake --build $BUILD_DIR --config Release

PATH=$PATH:${Qt_ROOT}/bin ci/deploywindows.sh

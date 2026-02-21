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

# configure build files with CMake
# we need to explicitly set the install prefix, as CMake's default is /usr/local for some reason...
cmake "$REPO_ROOT" -DCMAKE_BUILD_TYPE=Debug -DWITH_ORTOOLS=ON -DENABLE_COVERAGE=ON -DQt6_DIR=/opt/qt/6.8.2/gcc_64/lib/cmake/Qt6

# Wraps the compilation with the Build Wrapper to generate configuration (used
# later by the SonarQube Scanner) into the "bw-output" folder
/opt/build-wrapper-linux-x86/build-wrapper-linux-x86-64 \
	--out-dir bw-output cmake \
	--build .
# Test project
ctest -VV

# Generate coverage report
make gcov

# Scan project
/opt/sonar-scanner-6.2.1.4610-linux-x64/bin/sonar-scanner -Dsonar.host.url=https://sonarcloud.io -Dproject.settings="$REPO_ROOT"/sonar-project.properties -Dsonar.projectBaseDir="$REPO_ROOT" -Dsonar.cfamily.gcov.reportsPath="$BUILD_DIR"



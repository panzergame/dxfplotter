#! /bin/bash

set -x
set -e

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))

# the preset resolves its binary directory relative to the repo root
cd "$REPO_ROOT"

# configure build files with CMake
cmake --preset ci-build-test-coverage

# Wraps the compilation with the Build Wrapper to generate configuration (used
# later by the SonarQube Scanner) into the "bw-output" folder
/opt/build-wrapper-linux-x86/build-wrapper-linux-x86-64 \
	--out-dir bw-output cmake \
	--build build

# Test project
ctest --test-dir build -VV

# Generate coverage report
cmake --build build --target coverage

# Scan project
/opt/sonar-scanner-6.2.1.4610-linux-x64/bin/sonar-scanner -Dsonar.host.url=https://sonarcloud.io -Dproject.settings="$REPO_ROOT"/sonar-project.properties -Dsonar.projectBaseDir="$REPO_ROOT"

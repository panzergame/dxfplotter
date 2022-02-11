#! /bin/bash

set -x
set -e

# generate release name
COMMIT=$(git rev-parse --short HEAD)
TAG=$(git describe --tags)
RELEASE_NAME="dxfplotter-$TAG-$COMMIT-x86_64"

BUILD_DIR="build"
BINARY="${BUILD_DIR}/Release/dxfplotter.exe"
DEPLOY_DIR="${RELEASE_NAME}"

windeployqt --dir $DEPLOY_DIR $BINARY
cp $BINARY $DEPLOY_DIR

zip -r "${RELEASE_NAME}.zip" $RELEASE_NAME

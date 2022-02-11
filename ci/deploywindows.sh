#! /bin/bash

set -x
set -e

# generate release name
COMMIT=$(git rev-parse --short HEAD)
TAG=$(git describe --tags)
RELEASE_NAME="dxfplotter-$TAG-$COMMIT-x86_64"

BUILD_DIR="build"
BINARY_NAME="dxfplotter.exe"
BINARY_PATH="${BUILD_DIR}/Release/${BINARY_NAME}"

DEPLOY_DIR="${RELEASE_NAME}"
mkdir $DEPLOY_DIR
cp $BINARY_PATH $DEPLOY_DIR

windeployqt --dir $DEPLOY_DIR "${DEPLOY_DIR}/${BINARY_NAME}"

zip -r "${RELEASE_NAME}.zip" $RELEASE_NAME

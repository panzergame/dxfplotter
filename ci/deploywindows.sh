#! /bin/bash

set -x
set -e

# generate release name
COMMIT=$(git rev-parse --short HEAD)
BRANCH=$(git rev-parse --abbrev-ref HEAD)
VERSION=${BRANCH##*/}
RELEASE_NAME="dxfplotter-$VERSION-$COMMIT-x86_64-windows"

BUILD_DIR="build"
BINARY_NAME="dxfplotter.exe"
BINARY_PATH="${BUILD_DIR}/Release/${BINARY_NAME}"

DEPLOY_DIR="${RELEASE_NAME}"
mkdir $DEPLOY_DIR
cp $BINARY_PATH $DEPLOY_DIR

windeployqt --release --dir $DEPLOY_DIR "${DEPLOY_DIR}/${BINARY_NAME}"

zip -r "${RELEASE_NAME}.zip" $RELEASE_NAME

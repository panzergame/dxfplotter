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

BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" appimage-build-XXXXXX)

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
cmake "$REPO_ROOT" -DCMAKE_INSTALL_PREFIX=/usr

# build project and install files into AppDir
make -j$(nproc)
make install DESTDIR=AppDir

# now, build AppImage using linuxdeployqt
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage

# make them executable
chmod +x linuxdeploy*.AppImage

# linuxdeployqt seems to need desktop and icon files to be placed at the root of AppDir
cp AppDir/usr/share/icons/hicolor/256x256/apps/dxfplotter.png AppDir/usr/share/applications/dxfplotter.desktop AppDir

# generate the AppImage
# use -unsupported-allow-new-glibc for newest linux distribution
./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/bin/dxfplotter -appimage -extra-plugins=iconengines,platformthemes/libqgtk3.so -unsupported-allow-new-glibc

COMMIT=$(git rev-parse --short HEAD)
TAG=$(git describe --tags)
RELEASE_NAME="dxfplotter-$TAG-$COMMIT-x86_64"

# move built AppImage back into original CWD
mv dxfplotter*.AppImage "$OLD_CWD"/"$RELEASE_NAME".AppImage

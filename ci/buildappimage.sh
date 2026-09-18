#! /bin/bash

set -x
set -e

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))

# the preset resolves its binary directory relative to the repo root
cd "$REPO_ROOT"

git config --global --add safe.directory $REPO_ROOT

# generate release name
COMMIT=$(git rev-parse --short HEAD)
TAG=$(git describe --tags)
RELEASE_NAME="dxfplotter-$TAG-$COMMIT-x86_64-linux"

# configure build files with CMake
cmake --preset ci-deploy

# build project and install files into AppDir
cmake --build build
DESTDIR="$REPO_ROOT"/build/AppDir cmake --install build

# linuxdeployqt writes its output into the current directory
cd build

# now, build AppImage using linuxdeployqt
wget https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage

# make them executable
chmod +x linuxdeploy*.AppImage

# linuxdeployqt seems to need desktop and icon files to be placed at the root of AppDir
cp AppDir/usr/share/icons/hicolor/256x256/apps/dxfplotter.png AppDir/usr/share/applications/dxfplotter.desktop AppDir

# generate the AppImage
# use -unsupported-allow-new-glibc for newest linux distribution
./linuxdeployqt-continuous-x86_64.AppImage AppDir/usr/bin/dxfplotter -appimage -extra-plugins=iconengines,platformthemes/libqgtk3.so,renderers/libopenglrenderer.so -unsupported-allow-new-glibc 

# move built AppImage back into the repo root, where the release job picks it up
mv dxfplotter*.AppImage "$REPO_ROOT"/"$RELEASE_NAME".AppImage

#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

set -e


echo Downloading Qt
#DOWNLOAD_URL=https://download.qt.io/archive/qt/5.9/5.9.5/qt-opensource-mac-x64-5.9.5.dmg
brew update && brew install qt5 --with-qtwebengine
echo Qt installed

if [ $TRAVIS_BRANCH == "CI" ]
then
	BRANCH="master"
else
	BRANCH=$TRAVIS_BRANCH
fi

echo "Installing Rhoconnect client"
git clone -b master https://github.com/rhomobile/rhoconnect-client.git ../rhoconnect-client

git clone -b OpenSSL_1_1_0-stable https://github.com/tauplatform/openssl.git ../openssl
cd ../openssl
./tau_build_macos_lib.sh

echo "Building rhosim"
cd $TRAVIS_BUILD_DIR
rake build:osx:rhosimulator
# > build.log

OUT=$?

if [ $OUT -eq 0 ];then
   echo "RhoSimulator built successfully"
else
   echo "Error building RhoSimulator"
   cat build.log
fi

exit $OUT
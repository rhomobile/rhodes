#!/usr/bin/env bash
set -x

cp ./rhobuild.yml.example ./rhobuild.yml

set -e

echo Downloading Qt

wget -q https://s3.amazonaws.com/files.tau-platform.com/buildenv/Qt5.15.2_mini.tar.gz -O $PIPELINE_WORKSPACE/Qt5.15.2.tar.gz
tar -xzf $PIPELINE_WORKSPACE/Qt5.15.2.tar.gz -C $PIPELINE_WORKSPACE/
echo Qt installed

echo "Installing Rhoconnect client"
git clone -b master https://github.com/rhomobile/rhoconnect-client.git ../rhoconnect-client

git clone -b OpenSSL_1_1_0-stable https://github.com/tauplatform/openssl.git ../openssl
cd ../openssl
./tau_build_macos_lib.sh

echo "Building rhosim"
cd $TRAVIS_BUILD_DIR
#rm $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/RhoSimulator.app.zip
rm -rf $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/*
env
rake build:osx:rhosimulator --trace
cd $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/
zip -r -y RhoSimulator.app.zip RhoSimulator.app
# rm -rf $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/RhoSimulator.app
cd $TRAVIS_BUILD_DIR
# > build.log

OUT=$?

if [ $OUT -eq 0 ];then
   echo "RhoSimulator built successfully"
else
   echo "Error building RhoSimulator"
   cat build.log
fi

exit $OUT

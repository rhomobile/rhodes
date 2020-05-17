#!/usr/bin/env bash
set -x

#echo "Building rhosim"
#cd $TRAVIS_BUILD_DIR
#rm $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/RhoSimulator.app.zip
#rm -rf $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/*
#rake build:osx:rhosimulator
#cd $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/
#zip -r -y RhoSimulator.app.zip RhoSimulator.app
#rm -rf $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/RhoSimulator.app
#cd $TRAVIS_BUILD_DIR
# > build.log

#OUT=$?

#if [ $OUT -eq 0 ];then
#   echo "RhoSimulator built successfully"
#else
#   echo "Error building RhoSimulator"
#   cat build.log
#fi

#exit $OUT

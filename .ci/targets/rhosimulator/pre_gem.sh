#!/usr/bin/env bash

echo "Building rhosim is not supported yet"
exit 0

cp ./rhobuild.yml.example ./rhobuild.yml

echo "Installing Rhoconnect client"

set -e

if [ $TRAVIS_BRANCH == "travis" ]
then
	BRANCH="master"
else
	BRANCH=$TRAVIS_BRANCH
fi

git clone -b $BRANCH https://github.com/rhomobile/rhoconnect-client.git ../rhoconnect-client

rake build:rhosimulator > build.log
OUT=$?

git clean -fdx

if [ $OUT -eq 0 ];then
   echo "RhoSimulator built successfully"
else
   echo "Error building RhoSimulator"
   cat build.log
fi

exit $OUT
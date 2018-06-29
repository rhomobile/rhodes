#!/usr/bin/env bash
echo "Installing Rhoconnect client"

set -e

if [ $TRAVIS_BRANCH == "CI" ]
then
	BRANCH="master"
else
	BRANCH=$TRAVIS_BRANCH
fi

git clone https://github.com/rhomobile/rhoconnect-client.git $HOME/rhoconnect-client
cd $HOME/rhoconnect-client
rake gem:make_gem
gem install ./rhoconnect-client-$(cat ./version).gem --no-document
cd $TRAVIS_BUILD_DIR

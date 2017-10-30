#!/usr/bin/env bash
echo "Installing Rhoconnect client"

git clone -b $TRAVIS_BRANCH https://github.com/rhomobile/rhoconnect-client.git $HOME/rhoconnect-client
cd $HOME/rhoconnect-client
rake gem:make_gem
gem install ./rhoconnect-client-$(cat ./version).gem --no-document
cd $TRAVIS_BUILD_DIR

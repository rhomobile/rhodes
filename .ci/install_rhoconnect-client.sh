#!/usr/bin/env bash
echo "Installing Rhoconnect client"

set -e

BRANCH="5-5-stable"

git clone -b $BRANCH https://github.com/rhomobile/rhoconnect-client.git $HOME/rhoconnect-client
cd $HOME/rhoconnect-client
rake gem:make_gem
gem install ./rhoconnect-client-$(cat ./version).gem --no-document
cd $TRAVIS_BUILD_DIR

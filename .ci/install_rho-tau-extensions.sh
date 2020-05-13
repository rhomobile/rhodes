#!/usr/bin/env bash
echo "Installing Rho-Tau-Extensions"

set -e

if [ $TRAVIS_BRANCH == "CI" ]
then
	BRANCH="master"
else
	BRANCH=$TRAVIS_BRANCH
fi

git clone https://$GITHUB_TRAVIS_TOKEN:x-oauth-basic@github.com/tauplatform/rho-tau-extensions.git $HOME/rho-tau-extensions
cd $HOME/rho-tau-extensions
rake gem
gem install ./rho-tau-extensions-$(cat ./version).gem --no-document
cd $TRAVIS_BUILD_DIR

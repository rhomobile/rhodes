#!/usr/bin/env bash
set -x

RUBY_INSTALLED=$(rvm list | grep ruby-2.7.1)
echo $RUBY_INSTALLED
if [[ -z $RUBY_INSTALLED ]]; then
export HOMEBREW_NO_INSTALL_CLEANUP=1
#brew install openssl
rvm install --autolibs=disable 2.7.1
fi

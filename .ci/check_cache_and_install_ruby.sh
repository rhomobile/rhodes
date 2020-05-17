#!/usr/bin/env bash
set -x

RUBY_INSTALLED=$(rvm list | grep ruby-$RHO_RUBY)
echo $RUBY_INSTALLED
if [[ -z $RUBY_INSTALLED ]]; then
export HOMEBREW_NO_INSTALL_CLEANUP=1
#brew install openssl
rvm install --autolibs=disable $RHO_RUBY
fi

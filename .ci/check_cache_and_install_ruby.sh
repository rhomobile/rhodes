#!/usr/bin/env bash
set -x

RUBY_INSTALLED=$(rvm list | grep ruby-2.3.4)
rvm pkg install openssl
echo $RUBY_INSTALLED
if [[ -z $RUBY_INSTALLED ]]; then
rvm install 2.3.4
fi






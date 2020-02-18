#!/usr/bin/env bash
set -x

RUBY_INSTALLED=$(rvm list | grep ruby-2.3.4)
rvm pkg install openssl
echo $RUBY_INSTALLED
if [[ -z $RUBY_INSTALLED ]]; then
rvm install 2.3.4 --with-openssl-dir=$rvm_path/usr
ls /Users/travis/.rvm/log/
#DEBUG_LOG=$(ls /Users/travis/.rvm/log/*_ruby-2.3.4/rubygems.install.log | sort -V | tail -n 1)
cat $DEBUG_LOG
fi






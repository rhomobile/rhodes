#!/usr/bin/env bash
set -x

RUBY_INSTALLED=$(rvm list | grep ruby-2.3.4)
echo $RUBY_INSTALLED
if [[ -z $RUBY_INSTALLED ]]; then
rvm install 2.3.4
rvm use 2.3.4
rvm gemset create rhodes
rvm gemset use rhodes

rvm gemset list
gem list

gem install rest-client -v 1.7.3 --no-document
gem install listen -v 3.0.6 --no-document
gem install zip --no-document
gem install templater -v 1.0.0 --no-document
gem install rake -v 12.0.0 --no-document
gem install uuid -v 2.3.7 --no-document
gem install systemu -v 2.6.4 --no-document
gem install json -v 1.8.3 --no-document
gem install CFPropertyList -v 2.2.8 --no-document
gem install naturally -v 1.3.2 --no-document
gem install simctl -v 1.5.6 --no-document
gem install listen -v 3.0.6 --no-document
gem install rubyzip -v 1.2.1 --no-document
gem install ffi -v 1.9.14 --no-document
gem install rdoc -v 4.2.2 --no-document
gem install deep_merge -v 1.1.1 --no-document
gem install nokogiri -v 1.8.2 --no-document
fi


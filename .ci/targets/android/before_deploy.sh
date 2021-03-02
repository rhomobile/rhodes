#!/usr/bin/env bash

set -x

bundle info bundler
gem list bundler

gem uninstall bundler
gem install bundler -v 2.1.4

cp $HOME/$RHO_APP/$($TRAVIS_BUILD_DIR/.ci/app_build_root.rb)/bin/target/android/*.apk $BUILD_ARTEFACTS_DIR

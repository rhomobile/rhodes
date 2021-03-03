#!/usr/bin/env bash

set -x

cp $HOME/$RHO_APP/$($TRAVIS_BUILD_DIR/.ci/app_build_root.rb)/bin/target/android/*.apk $BUILD_ARTEFACTS_DIR

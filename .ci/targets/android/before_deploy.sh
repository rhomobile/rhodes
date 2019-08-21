#!/usr/bin/env bash

set -x

cp $HOME/$RHO_APP/$($TRAVIS_BUILD_DIR/.ci/app_build_root.rb)/bin/target/android/*.apk $BUILD_ARTEFACTS_DIR

if [[ -z $SKIP_TESTS ]]; then
    cp $HOME/$RHO_APP/$($TRAVIS_BUILD_DIR/.ci/app_build_root.rb)/RhoLogSpec.txt $BUILD_ARTEFACTS_DIR
fi
#!/usr/bin/env bash

set -x

$TRAVIS_BUILD_DIR/.ci/modify_target_abi.rb ./build.yml $ANDROID_ABIS
rake device:android:debug

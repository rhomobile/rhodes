#!/usr/bin/env bash

set -e
set -x

#setup for host
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/hosts/$OS_NAME/before_install.sh
#setup for target
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/before_install.sh
#install Ruby version specified in RHO_RUBY env var ( system Ruby by default )
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/check_cache_and_install_ruby.sh
rvm use $RHO_RUBY
# run target-specific and host-specific install scripts ( for instance download and install NDK/SDK for Android targets)
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/hosts/$OS_NAME/install.sh
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/install.sh

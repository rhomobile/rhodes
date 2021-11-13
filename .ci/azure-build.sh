#!/usr/bin/env bash

set -e
set -x

uname -a

#setup for host
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/hosts/$OS_NAME/before_install.sh
#source ~/.rvm/scripts/rvm
#rvm list
#setup for target
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/before_install.sh
#install Ruby version specified in RHO_RUBY env var ( system Ruby by default )
#$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/check_cache_and_install_ruby.sh
#rvm use $RHO_RUBY
# run target-specific and host-specific install scripts ( for instance download and install NDK/SDK for Android targets)
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/hosts/$OS_NAME/install.sh
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/install.sh

# start building of Rhodes gem in a separate gemset
cd $BUILD_DIR
#rvm gemset create rhodes
#rvm gemset use rhodes

bundle install
gem list

#TODO: build RhoRuby executable for host platform
#- .ci/hosts/$TRAVIS_OS_NAME/build_rhoruby.sh

# run target-specific and host-specific script before start of Rhodes gem build
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/hosts/$OS_NAME/pre_gem.sh
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/pre_gem.sh

# build Rhodes gem
$BUILD_DIR/lib/build/buildgem.rb

# install Rhodes gem to clean gemset. All dependencies should be installed automatically
gem install --no-document ./rhodes-$(cat ./version).gem

# check rhodes path is valid
#if [ "$(get-rhodes-info --rhodes-path)" != "$HOME/.rvm/gems/$RHO_RUBY@rhodes/gems/rhodes-$(cat ./version)" ]
#then
#    echo "Unexpected path to Rhodes:"
#    echo "$(get-rhodes-info --rhodes-path)"
#    echo "Expected path:"
#    echo "$HOME/.rvm/gems/$RHO_RUBY@rhodes/gems/rhodes-$(cat ./version)"
#    exit 1
#fi

# run target-specific rhodes-setup command
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/rho_setup.sh

# clone and install rhoconnect-client gem
.ci/install_rhoconnect-client.sh

# clone and install rhoconnect-client gem if required by matrix config
if [[ -n $RHO_TAU_EXTENSIONS ]]; then
    .ci/install_rho-tau-extensions.sh;
fi

# run target-specific script after all Rhodes build environment has been setup
$BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/post_gem.sh

gem list
cd $HOME

# generate few apps from scratch
rhodes app vanilla_rhodes_app
cd vanilla_rhodes_app 
#- safe_run_target_script upgrade_package.sh here we build not only android aps
rhodes extension testext
cd ..
rhodes jsapp vanilla_rhodes_jsapp
rhodes nodejsapp vanilla_rhodes_nodejsapp

# clone testable app if required by matrix config
if [[ -n $RHO_APP ]]; then
    git clone $($BUILD_DIR/.ci/app_repo.rb) $HOME/$RHO_APP;
    cd $HOME/$RHO_APP/$($BUILD_DIR/.ci/app_build_root.rb);
fi

if [[ -z $SKIP_BUILD ]]; then
    $BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/build_artefact_for_deploy.sh;
fi
#TODO: change this to run specs in auto mode and collect results
if [[ -z $SKIP_TESTS ]]; then
    travis_wait 45 $BUILD_DIR/.ci/safe_run.sh $BUILD_DIR/.ci/targets/$RHO_TARGET/$RHO_RUNNER_SCRIPT;
fi
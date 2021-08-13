set -x

$TRAVIS_BUILD_DIR/.ci/safe_run.sh $TRAVIS_BUILD_DIR/.ci/targets/$RHO_TARGET/$1

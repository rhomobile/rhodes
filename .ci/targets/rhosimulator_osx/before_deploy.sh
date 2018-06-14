#!/usr/bin/env bash

set -x

cp $TRAVIS_BUILD_DIR/platform/osx/bin/RhoSimulator/*.zip $BUILD_ARTEFACTS_DIR

export S3_DEPLOY_DIR=rhosimulator
export S3_JOB_DIR="$TRAVIS_COMMIT"
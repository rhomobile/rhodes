#!/usr/bin/env bash
set -x

cd $PIPELINE_WORKSPACE/vanilla_rhodes_app

rake run:iphone:rhosimulator

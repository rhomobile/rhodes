#!/usr/bin/env bash
set -x

rake build:iphone:rhodeslib_bundle["$HOME/$RHO_APP/Native_iOS"]

rake build:iphone:rhodeslib_framework["$HOME/$RHO_APP/Native_iOS"]

#!/usr/bin/env bash
set -x

rake build:iphone:rhodeslib_framework["$HOME/$RHO_APP/Native_iOS"]

if [ $? -eq 0 ]; then
    echo RHo: rhodes lib builded OK !
else
    echo RHO: rhodes lib failed !
    exit -1
fi

rake build:iphone:rhodeslib_bundle["$HOME/$RHO_APP/Native_iOS"]

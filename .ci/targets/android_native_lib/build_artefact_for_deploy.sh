#!/usr/bin/env bash

set -x

rake build:android:rhodeslib_lib["$HOME/$RHO_APP/Native_Android/MyApplication/app/libs/"]

if [ $? -eq 0 ]; then
    echo RHO: rhodes lib built ok !
else
    echo RHO: rhodes lib built failed !
    exit -1
fi

rake build:android:rhodeslib_bundle["$HOME/$RHO_APP/Native_Android/MyApplication/app/src/main/assets/"]

#rake device:android:debug

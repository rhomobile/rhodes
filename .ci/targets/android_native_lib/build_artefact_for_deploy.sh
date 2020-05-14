#!/usr/bin/env bash

set -x

rake build:android:rhodeslib_lib["$HOME/$RHO_APP/Native_Android/MyApplication/app/libs/"]

rake build:android:rhodeslib_bundle["$HOME/$RHO_APP/Native_Android/MyApplication/app/src/main/assets/"]

#rake device:android:debug

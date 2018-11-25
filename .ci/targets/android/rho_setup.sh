#!/usr/bin/env bash
set -x
ANDROID_NDK_HOME="${HOME}/android-ndk-r${NDK_NUMBER}"
printf "$JAVA_HOME\n$ANDROID_HOME\n$ANDROID_NDK_HOME\n\n\n\n\n\n\n" | rhodes-setup

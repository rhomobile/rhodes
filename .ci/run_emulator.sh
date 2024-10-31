#!/usr/bin/env bash

SDK_MANAGER=$1
VER_ANDROID=$2
temp_string=$($SDK_MANAGER --list | grep system-images |grep android-$VER_ANDROID | grep google_apis | grep -wv google_apis_playstore | grep x86 | grep -wv x86_64)

if [[ -z "$temp_string" ]]; then
    temp_string=$($SDK_MANAGER --list | grep system-images |grep android-$VER_ANDROID | grep google_apis | grep -wv google_apis_playstore | grep x86_64)
fi
echo "temo_string: $temp_string"

sdk_string=$(echo "$temp_string" | awk -F '|' '{print $1}' | tr -d '[:space:]')
echo "sdk_string: $sdk_string"

# Install AVD files
echo "y" | $SDK_MANAGER --install "$sdk_string"

# Create emulator
echo "no" | $ANDROID_HOME/tools/bin/avdmanager create avd -n xamarin_android_emulator -k "$sdk_string" --force

$ANDROID_HOME/emulator/emulator -list-avds

echo "Starting emulator"

# Start emulator in background
nohup $ANDROID_HOME/emulator/emulator -avd xamarin_android_emulator -no-snapshot > /dev/null 2>&1 &
$ANDROID_HOME/platform-tools/adb wait-for-device shell 'while [[ -z $(getprop sys.boot_completed | tr -d '\r') ]]; do sleep 1; done; input keyevent 82'

$ANDROID_HOME/platform-tools/adb devices

echo "Emulator started"
#!/usr/bin/env bash
set -x

#get Android NDK
wget -q https://dl.google.com/android/repository/android-ndk-r$NDK_NUMBER-linux-x86_64.zip -O $HOME/ndk.zip
unzip -q $HOME/ndk.zip -d $HOME
rm $HOME/ndk.zip
wget -q https://s3.amazonaws.com/files.tau-technologies.com/buildenv/addon-symbol-emdk_v4.2-API-22.zip -O $HOME/emdk.zip
unzip -q $HOME/emdk.zip -d $ANDROID_HOME/add-ons

#hack - remove android platform 25 since it has no corresponding ndk api.
rm -r $ANDROID_HOME/platforms/android-25

#genearate keystore
cd $HOME/.android && printf "android\nandroid\nTRAVIS-CI\nCI-SYSTEM\nTAU\nUNIVERSE\nUNIVERSE\nUN\nyes\n\n" | keytool -genkey -v -keystore debug.keystore -alias androiddebugkey -keyalg RSA -keysize 2048 -validity 10000
#download 

#EMULEVEL=${RHO_ANDROID_LEVEL:-26}
#EMULEVEL=$((EMULEVEL>25?25:EMULEVEL))

#echo yes | $ANDROID_HOME/tools/bin/sdkmanager "system-images;android-${EMULEVEL};default;armeabi-v7a"
#echo yes | $ANDROID_HOME/tools/bin/sdkmanager "system-images;android-${EMULEVEL};google_apis;armeabi-v7a"

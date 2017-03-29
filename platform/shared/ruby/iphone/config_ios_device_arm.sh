#!/bin/sh

ARCH=armv7

echo $ARCH

export CFLAGS="-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk -arch $ARCH -std=c99 -miphoneos-version-min=6.0"
export CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
#export LDFLAGS="--sysroot=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS10.3.sdk"
export LDFLAGS="-arch $ARCH"

./configure --with-arch="$ARCH" --without-gcc --host="$ARCH"
#!/bin/sh

ARCH=x86_64

echo $ARCH

export CFLAGS="-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator.sdk -arch $ARCH -std=c99 -miphoneos-version-min=6.0"
export CC=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
export LDFLAGS="-arch $ARCH"

./configure --with-arch="$ARCH" --without-gcc --host="$ARCH"
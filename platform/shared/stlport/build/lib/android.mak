SRCROOT := ..
COMPILER_NAME := gcc
TARGET_OS := arm-linux
STLPORT_INCLUDE_DIR = ../../stlport

ifeq ($(NDK_DIR),)
$(error Please run with NDK_DIR=your_android_ndk_root)
endif

TOOLCHAIN_PREFIX := $(NDK_DIR)/build/prebuilt/$(NDK_HOST)/arm-eabi-4.2.1/bin/arm-eabi-
CC := $(TOOLCHAIN_PREFIX)gcc
CXX := $(TOOLCHAIN_PREFIX)g++
AR := $(TOOLCHAIN_PREFIX)ar

include Makefile.inc
include $(SRCROOT)/Makefiles/top.mak

COMMON_FLAGS := \
       -DTARGET_OS=android -DANDROID -DOS_ANDROID \
       -DCOMPILER_NAME=gcc -DOSNAME=android -DBUILD_OSNAME=android \
       -mandroid -ffunction-sections -fdata-sections -Os -g \
       --sysroot=$(NDK_DIR)/build/platforms/android-3/arch-arm \
       -fPIC \
       -fvisibility=hidden \
       -D__NEW__
CFLAGS := $(COMMON_FLAGS)
CXXFLAGS := \
      $(COMMON_FLAGS) \
      -fno-rtti -fno-exceptions \
      -fvisibility-inlines-hidden

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

STLPORT_BASE    := $(HOME)/work/ndk-wrappers/stlport

LOCAL_MODULE    := rhodes
LOCAL_SRC_FILES := src/platform.cpp

LOCAL_CFLAGS    += -I$(STLPORT_BASE)/stlport \
                   -I$(LOCAL_PATH)/include \
                   -D__NEW__ \
                   -D__SGI_STL_INTERNAL_PAIR_H \
                   -DANDROID -DOS_ANDROID
LOCAL_LDLIBS    += -L$(STLPORT_BASE)/build/lib/obj/arm-linux-gcc/so \
                   -lstlport
LOCAL_ARM_MODE  := arm

include $(BUILD_SHARED_LIBRARY)

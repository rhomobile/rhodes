LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rholog
LOCAL_SRC_FILES := \
    RhoLogConf.cpp \
    RhoLog.cpp \
    RhoLogSink.cpp \
    RhoPlainLog.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

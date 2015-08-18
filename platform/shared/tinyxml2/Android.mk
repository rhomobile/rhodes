LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := tinyxml2
LOCAL_SRC_FILES := tinyxml2.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

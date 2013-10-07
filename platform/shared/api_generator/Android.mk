LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhoapi
LOCAL_SRC_FILES := js_helpers.cpp MethodResult.cpp StringifyHelper.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

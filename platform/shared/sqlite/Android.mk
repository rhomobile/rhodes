LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := sqlite
LOCAL_SRC_FILES := sqlite3.c
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

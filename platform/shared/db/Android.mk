LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhodb
LOCAL_SRC_FILES := \
    DBAdapter.cpp \
    DBAttrManager.cpp \
    DBResult.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)b $(SHARED_PATH_INC) $(SHARED_PATH_INC)/sqlite

include $(BUILD_STATIC_LIBRARY)

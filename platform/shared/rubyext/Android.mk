LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhorubyext
LOCAL_SRC_FILES := \
    GeoLocation.cpp \
    RhoAppAdapter.cpp \
    System.cpp
LOCAL_C_INCLUDES := \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/ruby/include \
    $(SHARED_PATH_INC)/ruby/linux

include $(BUILD_STATIC_LIBRARY)

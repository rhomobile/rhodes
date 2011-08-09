LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhonet
LOCAL_SRC_FILES := \
    AsyncHttp.cpp \
    CURLNetRequest.cpp \
    HttpServer.cpp \
    ssl.cpp \
    URI.cpp \
    INetRequest.cpp
LOCAL_C_INCLUDES := \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/curl/include
#    $(SHARED_PATH_INC)/ruby/include \
#    $(SHARED_PATH_INC)/ruby/linux

include $(BUILD_STATIC_LIBRARY)

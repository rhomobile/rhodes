LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := json
LOCAL_SRC_FILES := \
    JSONIterator.cpp \
    RJSONTokener.c \
    arraylist.c \
    json_debug.c \
    json_object.c \
    json_tokener.c \
    json_util.c \
    linkhash.c \
    printbuf.c
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

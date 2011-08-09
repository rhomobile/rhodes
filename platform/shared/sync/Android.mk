LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhoconnect
LOCAL_SRC_FILES := \
    ClientRegister.cpp \
    SyncEngine.cpp \
    SyncNotify.cpp \
    SyncSource.cpp \
    SyncThread.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(SHARED_PATH_INC) $(SHARED_PATH_INC)/sqlite

include $(BUILD_STATIC_LIBRARY)

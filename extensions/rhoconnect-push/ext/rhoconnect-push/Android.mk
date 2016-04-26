LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := anspush
LOCAL_SRC_FILES :=  platform/android/jni/src/anspush.cpp \
                    shared/ruby/anspush_wrap.c \
                    shared/src/anspushclient.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(PROJECT_PATH) \
					$(PROJECT_PATH)/include/rhodes/details \
					$(PROJECT_PATH)/include \
                    $(SHARED_PATH_INC) \
                    $(SHARED_PATH_INC)/ruby/include \
                    $(SHARED_PATH_INC)/ruby/android \
                    $(SHARED_PATH_INC)/ruby/generated

include $(BUILD_STATIC_LIBRARY)


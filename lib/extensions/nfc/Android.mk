LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := nfc
LOCAL_SRC_FILES :=  ext/nfc/platform/android/jni/src/nfc.cpp \
                    ext/nfc/shared/ruby/nfc_wrap.c

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(PROJECT_PATH)/include \
                    $(SHARED_PATH_INC) \
                    $(SHARED_PATH_INC)/common \
                    $(SHARED_PATH_INC)/ruby/include \
                    $(SHARED_PATH_INC)/ruby/linux \
                    $(SHARED_PATH_INC)/ruby/generated

include $(BUILD_STATIC_LIBRARY)


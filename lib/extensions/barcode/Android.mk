LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := barcode
LOCAL_SRC_FILES :=  ext/barcode/platform/android/jni/src/barcode.cpp \
                    ext/barcode/shared/ruby/barcode_wrap.c

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(PROJECT_PATH)/include \
                    $(SHARED_PATH_INC) \
                    $(SHARED_PATH_INC)/ruby/include \
                    $(SHARED_PATH_INC)/ruby/linux \
                    $(SHARED_PATH_INC)/ruby/generated

include $(BUILD_STATIC_LIBRARY)


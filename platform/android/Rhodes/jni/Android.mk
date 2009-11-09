LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

RHO_SHARED_PATH := $(LOCAL_PATH)/../../../shared
STLPORT_BASE    := $(RHO_SHARED_PATH)/stlport
RHO_BIN_PATH    := $(RHO_SHARED_PATH)/../../../infusionsoft-client/bin

LOCAL_MODULE    := rhodes
LOCAL_SRC_FILES := src/platform.cpp \
                   src/callbacks.cpp \
                   src/webview.cpp \
                   src/geolocation.cpp \
                   src/mapview.cpp \
                   src/alert.cpp \
                   src/camera.cpp \
                   src/datetimepicker.cpp \
                   src/nativebar.cpp \
                   src/phonebook.cpp \
                   src/syncengine.cpp

LOCAL_CFLAGS    += -I$(STLPORT_BASE)/stlport \
                   -I$(LOCAL_PATH)/include \
                   -I$(RHO_SHARED_PATH) \
                   -I$(RHO_SHARED_PATH)/sqlite \
                   -I$(RHO_SHARED_PATH)/ruby/include \
                   -I$(RHO_SHARED_PATH)/ruby/linux \
                   -D__NEW__ \
                   -D__SGI_STL_INTERNAL_PAIR_H \
                   -DANDROID -DOS_ANDROID
LOCAL_LDLIBS    += -L$(STLPORT_BASE)/build/lib/obj/arm-linux-gcc/so \
                   -L$(RHO_BIN_PATH) \
                   -lrhomain -lshttpd -lruby -lrhosync -lrhodb -lrhocommon -lrholog -ljson -lstlport -lsqlite -ldl
LOCAL_ARM_MODE  := arm

include $(BUILD_SHARED_LIBRARY)

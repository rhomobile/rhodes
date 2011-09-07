LOCAL_PATH := $(call my-dir)
PROJECT_PATH := $(LOCAL_PATH)
SHARED_PATH_INC := $(LOCAL_PATH)/../../../shared
SHARED_PATH := ../../../shared

include $(CLEAR_VARS)

LOCAL_MODULE    := rhodes
LOCAL_SRC_FILES := \
    src/RhoClassFactory.cpp \
    src/JNIRhoRuby.cpp \
    src/alert.cpp \
    src/bluetooth.cpp \
    src/callbacks.cpp \
    src/camera.cpp \
    src/datetimepicker.cpp \
    src/event.cpp \
    src/fileapi.cpp \
    src/geolocation.cpp \
    src/logconf.cpp \
    src/logger.cpp \
    src/mapview.cpp \
    src/menu.cpp \
    src/nativebar.cpp \
    src/nativeview.cpp \
    src/navbar.cpp \
    src/phonebook.cpp \
    src/rhoconf.cpp \
    src/rhodes.cpp \
    src/rhodesapp.cpp \
    src/rhodessystem.cpp \
    src/ringtones.cpp \
    src/signature.cpp \
    src/socketimpl.cpp \
    src/splashscreen.cpp \
    src/sslimpl.cpp \
    src/webview.cpp \
    src/rhocryptimpl.cpp \
    rainbow_stab.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/rhodes/details \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/common \
    $(SHARED_PATH_INC)/ruby/include \
    $(SHARED_PATH_INC)/ruby/android \
    $(SHARED_PATH_INC)/curl/include \
    $(SHARED_PATH_INC)/sqlite

LOCAL_STATIC_LIBRARIES := \
	ruby rhorubyext json rhomain rhocommon rhodb rhoconnect rhonet curl sqlite rholog \
	rhocommon rhonet rhostat rhorubyext \
	unzip
#barcode nfc

LOCAL_LDLIBS := -llog -ldl -lz

include $(BUILD_SHARED_LIBRARY)

#include $(LOCAL_PATH)/../../../../lib/extensions/barcode/ext/barcode/platform/android/jni/Android.mk

NDK_MODULE_PATH := $(SHARED_PATH_INC)
$(call import-module,sqlite)
$(call import-module,curl)
$(call import-module,ruby)
$(call import-module,rubyext)
$(call import-module,json)
$(call import-module,logging)
$(call import-module,common)
$(call import-module,net)
$(call import-module,statistic)
$(call import-module,unzip)
$(call import-module,db)
$(call import-module,sync)
#$(call import-module,barcode)
#$(call import-module,nfc)

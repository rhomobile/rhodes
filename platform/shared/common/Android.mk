LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhomain
LOCAL_SRC_FILES := RhodesApp.cpp RhodesAppBase.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := rhocommon
LOCAL_SRC_FILES := \
    app_build_configs.c \
    AppMenu.cpp \
    map/BaseMapEngine.cpp \
    map/ESRIMapEngine.cpp \
    map/GoogleMapEngine.cpp \
    map/OSMMapEngine.cpp \
    map/MapEngine.cpp \
    map/GeocodingMapEngine.cpp \
    PosixThreadImpl.cpp \
    RhoConf.cpp \
    RhoFile.cpp \
    RhoMath.cpp \
    rhoparams.cpp \
    RhoThread.cpp \
    SplashScreen.cpp \
    ThreadQueue.cpp \
    Tokenizer.cpp \
    RhoTime.cpp \
    BundleManager.cpp
LOCAL_C_INCLUDES := $(SHARED_PATH_INC)
    #$(SHARED_PATH_INC)/curl/include
#    $(SHARED_PATH_INC)/ruby/include \
#    $(SHARED_PATH_INC)/ruby/linux

include $(BUILD_STATIC_LIBRARY)

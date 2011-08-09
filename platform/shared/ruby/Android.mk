LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ruby
LOCAL_SRC_FILES := \
    array.c \
    bignum.c \
    blockinlining.c \
    class.c \
    compar.c \
    compile.c \
    complex.c \
    cont.c \
    debug.c \
    dir.c \
    dln.c \
    dmyencoding.c \
    dmyext.c \
    enc/ascii.c \
    enc/unicode.c \
    enc/us_ascii.c \
    enc/utf_8.c \
    enum.c \
    enumerator.c \
    error.c \
    eval.c \
    ext/alert/alert_wrap.c \
    ext/asynchttp/asynchttp_wrap.c \
    ext/bluetooth/bluetooth_wrap.c \
    ext/nativeviewmanager/nativeviewmanager_wrap.c \
    ext/calendar/calendar_wrap.c \
    ext/calendar/event_wrap.c \
    ext/camera/camera_wrap.c \
    ext/datetimepicker/datetimepicker_wrap.c \
    ext/geolocation/geolocation_wrap.c \
    ext/mapview/mapview_wrap.c \
    ext/nativebar/nativebar_wrap.c \
    ext/navbar/navbar_wrap.c \
    ext/phonebook/phonebook_wrap.c \
    ext/rho/extensions.c \
    ext/rho/rhoruby.c \
    ext/rho/rhosupport.c \
    ext/rhoconf/rhoconf_wrap.c \
    ext/ringtones/ringtones_wrap.c \
    ext/signature/signature_wrap.c \
    ext/socket/socket.c \
    ext/sqlite3_api/sqlite3_api_wrap.c \
    ext/stringio/stringio.c \
    ext/strscan/strscan.c \
    ext/syncengine/syncengine_wrap.c \
    ext/system/system_wrap.c \
    ext/webview/webview_wrap.c \
    file.c \
    gc.c \
    generated/parse.c \
    hash.c \
    inits.c \
    io.c \
    iseq.c \
    load.c \
    marshal.c \
    math.c \
    miniprelude.c \
    missing/acosh.c \
    missing/cbrt.c \
    missing/crypt.c \
    missing/dup2.c \
    missing/erf.c \
    missing/hypot.c \
    missing/lgamma_r.c \
    missing/stdlib.c \
    missing/strlcat.c \
    missing/strlcpy.c \
    missing/tgamma.c \
    newline.c \
    numeric.c \
    object.c \
    pack.c \
    proc.c \
    process.c \
    random.c \
    range.c \
    rational.c \
    re.c \
    regcomp.c \
    regenc.c \
    regerror.c \
    regexec.c \
    regparse.c \
    regsyntax.c \
    ruby.c \
    safe.c \
    signal.c \
    sprintf.c \
    st.c \
    strftime.c \
    string.c \
    struct.c \
    thread.c \
    time.c \
    transcode.c \
    util.c \
    variable.c \
    version.c \
    vm.c \
    vm_dump.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/linux \
    $(LOCAL_PATH)/generated \
    $(LOCAL_PATH) \
    $(SHARED_PATH_INC) \
    $(SHARED_PATH_INC)/sqlite
    

include $(BUILD_STATIC_LIBRARY)

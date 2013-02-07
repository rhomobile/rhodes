
#include "ruby.h"

static VALUE rb_mParent;
static VALUE rb_cSystem;

VALUE rb_System_getPlatform(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getPlatform(int argc, VALUE *argv);
VALUE rb_System_getHasCamera(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getHasCamera(int argc, VALUE *argv);
VALUE rb_System_getScreenWidth(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getScreenWidth(int argc, VALUE *argv);
VALUE rb_System_getScreenHeight(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getScreenHeight(int argc, VALUE *argv);
VALUE rb_System_getRealScreenWidth(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getRealScreenWidth(int argc, VALUE *argv);
VALUE rb_System_getRealScreenHeight(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getRealScreenHeight(int argc, VALUE *argv);
VALUE rb_System_getScreenOrientation(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getScreenOrientation(int argc, VALUE *argv);
VALUE rb_System_getPpiX(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getPpiX(int argc, VALUE *argv);
VALUE rb_System_getPpiY(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getPpiY(int argc, VALUE *argv);
VALUE rb_System_getPhoneNumber(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getPhoneNumber(int argc, VALUE *argv);
VALUE rb_System_getDeviceOwnerEmail(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getDeviceOwnerEmail(int argc, VALUE *argv);
VALUE rb_System_getDeviceOwnerName(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getDeviceOwnerName(int argc, VALUE *argv);
VALUE rb_System_getDeviceId(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getDeviceId(int argc, VALUE *argv);
VALUE rb_System_getPhoneId(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getPhoneId(int argc, VALUE *argv);
VALUE rb_System_getDeviceName(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getDeviceName(int argc, VALUE *argv);
VALUE rb_System_getOsVersion(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getOsVersion(int argc, VALUE *argv);
VALUE rb_System_getLocale(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getLocale(int argc, VALUE *argv);
VALUE rb_System_getCountry(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getCountry(int argc, VALUE *argv);
VALUE rb_System_getIsEmulator(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getIsEmulator(int argc, VALUE *argv);
VALUE rb_System_getHasCalendar(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getHasCalendar(int argc, VALUE *argv);
VALUE rb_System_getIsMotorolaDevice(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getIsMotorolaDevice(int argc, VALUE *argv);
VALUE rb_System_getOemInfo(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getOemInfo(int argc, VALUE *argv);
VALUE rb_System_getUuid(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getUuid(int argc, VALUE *argv);
VALUE rb_System_getApplicationIconBadge(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getApplicationIconBadge(int argc, VALUE *argv);
VALUE rb_System_setApplicationIconBadge(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setApplicationIconBadge(int argc, VALUE *argv);
VALUE rb_System_getHttpProxyURI(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getHttpProxyURI(int argc, VALUE *argv);
VALUE rb_System_setHttpProxyURI(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setHttpProxyURI(int argc, VALUE *argv);
VALUE rb_System_getLockWindowSize(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getLockWindowSize(int argc, VALUE *argv);
VALUE rb_System_setLockWindowSize(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setLockWindowSize(int argc, VALUE *argv);
VALUE rb_System_getShowKeyboard(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getShowKeyboard(int argc, VALUE *argv);
VALUE rb_System_setShowKeyboard(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setShowKeyboard(int argc, VALUE *argv);
VALUE rb_System_getFullScreen(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getFullScreen(int argc, VALUE *argv);
VALUE rb_System_setFullScreen(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setFullScreen(int argc, VALUE *argv);
VALUE rb_System_getLocalServerPort(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getLocalServerPort(int argc, VALUE *argv);
VALUE rb_System_setLocalServerPort(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setLocalServerPort(int argc, VALUE *argv);
VALUE rb_System_getScreenAutoRotate(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getScreenAutoRotate(int argc, VALUE *argv);
VALUE rb_System_setScreenAutoRotate(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setScreenAutoRotate(int argc, VALUE *argv);
VALUE rb_System_applicationInstall(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_applicationInstall(int argc, VALUE *argv);
VALUE rb_System_isApplicationInstalled(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_isApplicationInstalled(int argc, VALUE *argv);
VALUE rb_System_applicationUninstall(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_applicationUninstall(int argc, VALUE *argv);
VALUE rb_System_openUrl(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_openUrl(int argc, VALUE *argv);
VALUE rb_System_runApplication(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_runApplication(int argc, VALUE *argv);
VALUE rb_System_unzipFile(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_unzipFile(int argc, VALUE *argv);
VALUE rb_System_zipFile(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_zipFile(int argc, VALUE *argv);
VALUE rb_System_zipFiles(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_zipFiles(int argc, VALUE *argv);
VALUE rb_System_setRegistrySetting(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setRegistrySetting(int argc, VALUE *argv);
VALUE rb_System_getRegistrySetting(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getRegistrySetting(int argc, VALUE *argv);
VALUE rb_System_setWindowFrame(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setWindowFrame(int argc, VALUE *argv);
VALUE rb_System_setWindowPosition(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setWindowPosition(int argc, VALUE *argv);
VALUE rb_System_getProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getProperty(int argc, VALUE *argv);
VALUE rb_System_getProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getProperties(int argc, VALUE *argv);
VALUE rb_System_getAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_getAllProperties(int argc, VALUE *argv);
VALUE rb_System_setProperty(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setProperty(int argc, VALUE *argv);
VALUE rb_System_setProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_setProperties(int argc, VALUE *argv);
VALUE rb_System_clearAllProperties(int argc, VALUE *argv, VALUE obj);
VALUE rb_s_System_def_clearAllProperties(int argc, VALUE *argv);



VALUE rb_System_s_default(VALUE klass);
VALUE rb_System_s_setDefault(VALUE klass, VALUE obj);


VALUE getRuby_System_Module(){ return rb_cSystem; }



void Init_RubyAPI_System(void)
{

    VALUE tmpParent = Qnil;
    rb_mParent = rb_define_module("Rho");
    

	rb_cSystem = rb_define_class_under(rb_mParent, "System", rb_cObject);

    //Constructor should be not available
	//rb_define_alloc_func(rb_cBarcode1, rb_barcode1_allocate);
    rb_undef_alloc_func(rb_cSystem);

rb_define_method(rb_cSystem, "getPlatform", rb_System_getPlatform, -1);
rb_define_singleton_method(rb_cSystem, "getPlatform", rb_s_System_def_getPlatform, -1);
rb_define_method(rb_cSystem, "getHasCamera", rb_System_getHasCamera, -1);
rb_define_singleton_method(rb_cSystem, "getHasCamera", rb_s_System_def_getHasCamera, -1);
rb_define_method(rb_cSystem, "getScreenWidth", rb_System_getScreenWidth, -1);
rb_define_singleton_method(rb_cSystem, "getScreenWidth", rb_s_System_def_getScreenWidth, -1);
rb_define_method(rb_cSystem, "getScreenHeight", rb_System_getScreenHeight, -1);
rb_define_singleton_method(rb_cSystem, "getScreenHeight", rb_s_System_def_getScreenHeight, -1);
rb_define_method(rb_cSystem, "getRealScreenWidth", rb_System_getRealScreenWidth, -1);
rb_define_singleton_method(rb_cSystem, "getRealScreenWidth", rb_s_System_def_getRealScreenWidth, -1);
rb_define_method(rb_cSystem, "getRealScreenHeight", rb_System_getRealScreenHeight, -1);
rb_define_singleton_method(rb_cSystem, "getRealScreenHeight", rb_s_System_def_getRealScreenHeight, -1);
rb_define_method(rb_cSystem, "getScreenOrientation", rb_System_getScreenOrientation, -1);
rb_define_singleton_method(rb_cSystem, "getScreenOrientation", rb_s_System_def_getScreenOrientation, -1);
rb_define_method(rb_cSystem, "getPpiX", rb_System_getPpiX, -1);
rb_define_singleton_method(rb_cSystem, "getPpiX", rb_s_System_def_getPpiX, -1);
rb_define_method(rb_cSystem, "getPpiY", rb_System_getPpiY, -1);
rb_define_singleton_method(rb_cSystem, "getPpiY", rb_s_System_def_getPpiY, -1);
rb_define_method(rb_cSystem, "getPhoneNumber", rb_System_getPhoneNumber, -1);
rb_define_singleton_method(rb_cSystem, "getPhoneNumber", rb_s_System_def_getPhoneNumber, -1);
rb_define_method(rb_cSystem, "getDeviceOwnerEmail", rb_System_getDeviceOwnerEmail, -1);
rb_define_singleton_method(rb_cSystem, "getDeviceOwnerEmail", rb_s_System_def_getDeviceOwnerEmail, -1);
rb_define_method(rb_cSystem, "getDeviceOwnerName", rb_System_getDeviceOwnerName, -1);
rb_define_singleton_method(rb_cSystem, "getDeviceOwnerName", rb_s_System_def_getDeviceOwnerName, -1);
rb_define_method(rb_cSystem, "getDeviceId", rb_System_getDeviceId, -1);
rb_define_singleton_method(rb_cSystem, "getDeviceId", rb_s_System_def_getDeviceId, -1);
rb_define_method(rb_cSystem, "getPhoneId", rb_System_getPhoneId, -1);
rb_define_singleton_method(rb_cSystem, "getPhoneId", rb_s_System_def_getPhoneId, -1);
rb_define_method(rb_cSystem, "getDeviceName", rb_System_getDeviceName, -1);
rb_define_singleton_method(rb_cSystem, "getDeviceName", rb_s_System_def_getDeviceName, -1);
rb_define_method(rb_cSystem, "getOsVersion", rb_System_getOsVersion, -1);
rb_define_singleton_method(rb_cSystem, "getOsVersion", rb_s_System_def_getOsVersion, -1);
rb_define_method(rb_cSystem, "getLocale", rb_System_getLocale, -1);
rb_define_singleton_method(rb_cSystem, "getLocale", rb_s_System_def_getLocale, -1);
rb_define_method(rb_cSystem, "getCountry", rb_System_getCountry, -1);
rb_define_singleton_method(rb_cSystem, "getCountry", rb_s_System_def_getCountry, -1);
rb_define_method(rb_cSystem, "getIsEmulator", rb_System_getIsEmulator, -1);
rb_define_singleton_method(rb_cSystem, "getIsEmulator", rb_s_System_def_getIsEmulator, -1);
rb_define_method(rb_cSystem, "getHasCalendar", rb_System_getHasCalendar, -1);
rb_define_singleton_method(rb_cSystem, "getHasCalendar", rb_s_System_def_getHasCalendar, -1);
rb_define_method(rb_cSystem, "getIsMotorolaDevice", rb_System_getIsMotorolaDevice, -1);
rb_define_singleton_method(rb_cSystem, "getIsMotorolaDevice", rb_s_System_def_getIsMotorolaDevice, -1);
rb_define_method(rb_cSystem, "getOemInfo", rb_System_getOemInfo, -1);
rb_define_singleton_method(rb_cSystem, "getOemInfo", rb_s_System_def_getOemInfo, -1);
rb_define_method(rb_cSystem, "getUuid", rb_System_getUuid, -1);
rb_define_singleton_method(rb_cSystem, "getUuid", rb_s_System_def_getUuid, -1);
rb_define_method(rb_cSystem, "getApplicationIconBadge", rb_System_getApplicationIconBadge, -1);
rb_define_singleton_method(rb_cSystem, "getApplicationIconBadge", rb_s_System_def_getApplicationIconBadge, -1);
rb_define_method(rb_cSystem, "setApplicationIconBadge", rb_System_setApplicationIconBadge, -1);
rb_define_singleton_method(rb_cSystem, "setApplicationIconBadge", rb_s_System_def_setApplicationIconBadge, -1);
rb_define_method(rb_cSystem, "getHttpProxyURI", rb_System_getHttpProxyURI, -1);
rb_define_singleton_method(rb_cSystem, "getHttpProxyURI", rb_s_System_def_getHttpProxyURI, -1);
rb_define_method(rb_cSystem, "setHttpProxyURI", rb_System_setHttpProxyURI, -1);
rb_define_singleton_method(rb_cSystem, "setHttpProxyURI", rb_s_System_def_setHttpProxyURI, -1);
rb_define_method(rb_cSystem, "getLockWindowSize", rb_System_getLockWindowSize, -1);
rb_define_singleton_method(rb_cSystem, "getLockWindowSize", rb_s_System_def_getLockWindowSize, -1);
rb_define_method(rb_cSystem, "setLockWindowSize", rb_System_setLockWindowSize, -1);
rb_define_singleton_method(rb_cSystem, "setLockWindowSize", rb_s_System_def_setLockWindowSize, -1);
rb_define_method(rb_cSystem, "getShowKeyboard", rb_System_getShowKeyboard, -1);
rb_define_singleton_method(rb_cSystem, "getShowKeyboard", rb_s_System_def_getShowKeyboard, -1);
rb_define_method(rb_cSystem, "setShowKeyboard", rb_System_setShowKeyboard, -1);
rb_define_singleton_method(rb_cSystem, "setShowKeyboard", rb_s_System_def_setShowKeyboard, -1);
rb_define_method(rb_cSystem, "getFullScreen", rb_System_getFullScreen, -1);
rb_define_singleton_method(rb_cSystem, "getFullScreen", rb_s_System_def_getFullScreen, -1);
rb_define_method(rb_cSystem, "setFullScreen", rb_System_setFullScreen, -1);
rb_define_singleton_method(rb_cSystem, "setFullScreen", rb_s_System_def_setFullScreen, -1);
rb_define_method(rb_cSystem, "getLocalServerPort", rb_System_getLocalServerPort, -1);
rb_define_singleton_method(rb_cSystem, "getLocalServerPort", rb_s_System_def_getLocalServerPort, -1);
rb_define_method(rb_cSystem, "setLocalServerPort", rb_System_setLocalServerPort, -1);
rb_define_singleton_method(rb_cSystem, "setLocalServerPort", rb_s_System_def_setLocalServerPort, -1);
rb_define_method(rb_cSystem, "getScreenAutoRotate", rb_System_getScreenAutoRotate, -1);
rb_define_singleton_method(rb_cSystem, "getScreenAutoRotate", rb_s_System_def_getScreenAutoRotate, -1);
rb_define_method(rb_cSystem, "setScreenAutoRotate", rb_System_setScreenAutoRotate, -1);
rb_define_singleton_method(rb_cSystem, "setScreenAutoRotate", rb_s_System_def_setScreenAutoRotate, -1);
rb_define_method(rb_cSystem, "applicationInstall", rb_System_applicationInstall, -1);
rb_define_singleton_method(rb_cSystem, "applicationInstall", rb_s_System_def_applicationInstall, -1);
rb_define_method(rb_cSystem, "isApplicationInstalled", rb_System_isApplicationInstalled, -1);
rb_define_singleton_method(rb_cSystem, "isApplicationInstalled", rb_s_System_def_isApplicationInstalled, -1);
rb_define_method(rb_cSystem, "applicationUninstall", rb_System_applicationUninstall, -1);
rb_define_singleton_method(rb_cSystem, "applicationUninstall", rb_s_System_def_applicationUninstall, -1);
rb_define_method(rb_cSystem, "openUrl", rb_System_openUrl, -1);
rb_define_singleton_method(rb_cSystem, "openUrl", rb_s_System_def_openUrl, -1);
rb_define_method(rb_cSystem, "runApplication", rb_System_runApplication, -1);
rb_define_singleton_method(rb_cSystem, "runApplication", rb_s_System_def_runApplication, -1);
rb_define_method(rb_cSystem, "unzipFile", rb_System_unzipFile, -1);
rb_define_singleton_method(rb_cSystem, "unzipFile", rb_s_System_def_unzipFile, -1);
rb_define_method(rb_cSystem, "zipFile", rb_System_zipFile, -1);
rb_define_singleton_method(rb_cSystem, "zipFile", rb_s_System_def_zipFile, -1);
rb_define_method(rb_cSystem, "zipFiles", rb_System_zipFiles, -1);
rb_define_singleton_method(rb_cSystem, "zipFiles", rb_s_System_def_zipFiles, -1);
rb_define_method(rb_cSystem, "setRegistrySetting", rb_System_setRegistrySetting, -1);
rb_define_singleton_method(rb_cSystem, "setRegistrySetting", rb_s_System_def_setRegistrySetting, -1);
rb_define_method(rb_cSystem, "getRegistrySetting", rb_System_getRegistrySetting, -1);
rb_define_singleton_method(rb_cSystem, "getRegistrySetting", rb_s_System_def_getRegistrySetting, -1);
rb_define_method(rb_cSystem, "setWindowFrame", rb_System_setWindowFrame, -1);
rb_define_singleton_method(rb_cSystem, "setWindowFrame", rb_s_System_def_setWindowFrame, -1);
rb_define_method(rb_cSystem, "setWindowPosition", rb_System_setWindowPosition, -1);
rb_define_singleton_method(rb_cSystem, "setWindowPosition", rb_s_System_def_setWindowPosition, -1);
rb_define_method(rb_cSystem, "getProperty", rb_System_getProperty, -1);
rb_define_singleton_method(rb_cSystem, "getProperty", rb_s_System_def_getProperty, -1);
rb_define_method(rb_cSystem, "getProperties", rb_System_getProperties, -1);
rb_define_singleton_method(rb_cSystem, "getProperties", rb_s_System_def_getProperties, -1);
rb_define_method(rb_cSystem, "getAllProperties", rb_System_getAllProperties, -1);
rb_define_singleton_method(rb_cSystem, "getAllProperties", rb_s_System_def_getAllProperties, -1);
rb_define_method(rb_cSystem, "setProperty", rb_System_setProperty, -1);
rb_define_singleton_method(rb_cSystem, "setProperty", rb_s_System_def_setProperty, -1);
rb_define_method(rb_cSystem, "setProperties", rb_System_setProperties, -1);
rb_define_singleton_method(rb_cSystem, "setProperties", rb_s_System_def_setProperties, -1);
rb_define_method(rb_cSystem, "clearAllProperties", rb_System_clearAllProperties, -1);
rb_define_singleton_method(rb_cSystem, "clearAllProperties", rb_s_System_def_clearAllProperties, -1);



    rb_define_singleton_method(rb_cSystem, "default", rb_System_s_default, 0);
    rb_define_singleton_method(rb_cSystem, "setDefault", rb_System_s_setDefault, 1);



    rb_const_set(rb_cSystem, rb_intern("PLATFORM_WM_CE"), rb_str_new2("WINDOWS") );
    rb_const_set(rb_cSystem, rb_intern("PLATFORM_ANDROID"), rb_str_new2("ANDROID") );
    rb_const_set(rb_cSystem, rb_intern("PLATFORM_IOS"), rb_str_new2("APPLE") );
    rb_const_set(rb_cSystem, rb_intern("PLATFORM_WP8"), rb_str_new2("WP8") );
    rb_const_set(rb_cSystem, rb_intern("PLATFORM_WINDOWS_DESKTOP"), rb_str_new2("WINDOWS_DESKTOP") );
    rb_const_set(rb_cSystem, rb_intern("SCREEN_PORTRAIT"), rb_str_new2("portrait") );
    rb_const_set(rb_cSystem, rb_intern("SCREEN_LANDSCAPE"), rb_str_new2("landscape") );
}


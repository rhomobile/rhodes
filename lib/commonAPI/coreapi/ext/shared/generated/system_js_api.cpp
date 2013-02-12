#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"


rho::String js_System_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getHasCamera(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getHasCamera(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getRealScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getRealScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getRealScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getRealScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getScreenOrientation(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getScreenOrientation(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getPpiX(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getPpiX(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getPpiY(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getPpiY(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getPhoneNumber(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getPhoneNumber(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getDeviceOwnerEmail(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getDeviceOwnerEmail(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getDeviceOwnerName(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getDeviceOwnerName(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getDevicePushId(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getDevicePushId(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getPhoneId(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getPhoneId(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getDeviceName(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getDeviceName(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getOsVersion(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getOsVersion(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getLocale(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getLocale(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getCountry(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getCountry(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getIsEmulator(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getIsEmulator(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getHasCalendar(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getHasCalendar(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getIsMotorolaDevice(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getIsMotorolaDevice(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getOemInfo(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getOemInfo(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getUuid(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getUuid(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getFullScreen(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getFullScreen(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setFullScreen(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setFullScreen(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getLocalServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getLocalServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setLocalServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setLocalServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getFreeServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getFreeServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getHasTouchscreen(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getHasTouchscreen(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getSecurityTokenNotPassed(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getSecurityTokenNotPassed(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getStartParams(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getStartParams(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_openUrl(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_openUrl(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_runApplication(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_runApplication(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_unzipFile(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_unzipFile(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_zipFile(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_zipFile(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_zipFiles(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_zipFiles(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setWindowFrame(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setWindowFrame(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setWindowPosition(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setWindowPosition(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);



rho::String js_s_System_getDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_setDefaultID(rho::json::CJSONArray& argv, const rho::String& strObjID);



extern "C" void Init_JSAPI_System(void)
{

    rho::apiGenerator::js_define_method("Rho:System:platform", js_System_getPlatform);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_platform", js_s_System_def_getPlatform);;

    rho::apiGenerator::js_define_method("Rho:System:hasCamera", js_System_getHasCamera);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_hasCamera", js_s_System_def_getHasCamera);;

    rho::apiGenerator::js_define_method("Rho:System:screenWidth", js_System_getScreenWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_screenWidth", js_s_System_def_getScreenWidth);;

    rho::apiGenerator::js_define_method("Rho:System:screenHeight", js_System_getScreenHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_screenHeight", js_s_System_def_getScreenHeight);;

    rho::apiGenerator::js_define_method("Rho:System:realScreenWidth", js_System_getRealScreenWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_realScreenWidth", js_s_System_def_getRealScreenWidth);;

    rho::apiGenerator::js_define_method("Rho:System:realScreenHeight", js_System_getRealScreenHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_realScreenHeight", js_s_System_def_getRealScreenHeight);;

    rho::apiGenerator::js_define_method("Rho:System:screenOrientation", js_System_getScreenOrientation);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_screenOrientation", js_s_System_def_getScreenOrientation);;

    rho::apiGenerator::js_define_method("Rho:System:ppiX", js_System_getPpiX);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_ppiX", js_s_System_def_getPpiX);;

    rho::apiGenerator::js_define_method("Rho:System:ppiY", js_System_getPpiY);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_ppiY", js_s_System_def_getPpiY);;

    rho::apiGenerator::js_define_method("Rho:System:phoneNumber", js_System_getPhoneNumber);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_phoneNumber", js_s_System_def_getPhoneNumber);;

    rho::apiGenerator::js_define_method("Rho:System:deviceOwnerEmail", js_System_getDeviceOwnerEmail);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_deviceOwnerEmail", js_s_System_def_getDeviceOwnerEmail);;

    rho::apiGenerator::js_define_method("Rho:System:deviceOwnerName", js_System_getDeviceOwnerName);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_deviceOwnerName", js_s_System_def_getDeviceOwnerName);;

    rho::apiGenerator::js_define_method("Rho:System:devicePushId", js_System_getDevicePushId);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_devicePushId", js_s_System_def_getDevicePushId);;

    rho::apiGenerator::js_define_method("Rho:System:phoneId", js_System_getPhoneId);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_phoneId", js_s_System_def_getPhoneId);;

    rho::apiGenerator::js_define_method("Rho:System:deviceName", js_System_getDeviceName);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_deviceName", js_s_System_def_getDeviceName);;

    rho::apiGenerator::js_define_method("Rho:System:osVersion", js_System_getOsVersion);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_osVersion", js_s_System_def_getOsVersion);;

    rho::apiGenerator::js_define_method("Rho:System:locale", js_System_getLocale);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_locale", js_s_System_def_getLocale);;

    rho::apiGenerator::js_define_method("Rho:System:country", js_System_getCountry);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_country", js_s_System_def_getCountry);;

    rho::apiGenerator::js_define_method("Rho:System:isEmulator", js_System_getIsEmulator);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_isEmulator", js_s_System_def_getIsEmulator);;

    rho::apiGenerator::js_define_method("Rho:System:hasCalendar", js_System_getHasCalendar);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_hasCalendar", js_s_System_def_getHasCalendar);;

    rho::apiGenerator::js_define_method("Rho:System:isMotorolaDevice", js_System_getIsMotorolaDevice);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_isMotorolaDevice", js_s_System_def_getIsMotorolaDevice);;

    rho::apiGenerator::js_define_method("Rho:System:oemInfo", js_System_getOemInfo);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_oemInfo", js_s_System_def_getOemInfo);;

    rho::apiGenerator::js_define_method("Rho:System:uuid", js_System_getUuid);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_uuid", js_s_System_def_getUuid);;

    rho::apiGenerator::js_define_method("Rho:System:applicationIconBadge", js_System_getApplicationIconBadge);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_applicationIconBadge", js_s_System_def_getApplicationIconBadge);;

    rho::apiGenerator::js_define_method("Rho:System:applicationIconBadge=", js_System_setApplicationIconBadge);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_applicationIconBadge=", js_s_System_def_setApplicationIconBadge);;

    rho::apiGenerator::js_define_method("Rho:System:httpProxyURI", js_System_getHttpProxyURI);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_httpProxyURI", js_s_System_def_getHttpProxyURI);;

    rho::apiGenerator::js_define_method("Rho:System:httpProxyURI=", js_System_setHttpProxyURI);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_httpProxyURI=", js_s_System_def_setHttpProxyURI);;

    rho::apiGenerator::js_define_method("Rho:System:lockWindowSize", js_System_getLockWindowSize);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_lockWindowSize", js_s_System_def_getLockWindowSize);;

    rho::apiGenerator::js_define_method("Rho:System:lockWindowSize=", js_System_setLockWindowSize);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_lockWindowSize=", js_s_System_def_setLockWindowSize);;

    rho::apiGenerator::js_define_method("Rho:System:showKeyboard", js_System_getShowKeyboard);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_showKeyboard", js_s_System_def_getShowKeyboard);;

    rho::apiGenerator::js_define_method("Rho:System:showKeyboard=", js_System_setShowKeyboard);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_showKeyboard=", js_s_System_def_setShowKeyboard);;

    rho::apiGenerator::js_define_method("Rho:System:fullScreen", js_System_getFullScreen);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_fullScreen", js_s_System_def_getFullScreen);;

    rho::apiGenerator::js_define_method("Rho:System:fullScreen=", js_System_setFullScreen);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_fullScreen=", js_s_System_def_setFullScreen);;

    rho::apiGenerator::js_define_method("Rho:System:localServerPort", js_System_getLocalServerPort);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_localServerPort", js_s_System_def_getLocalServerPort);;

    rho::apiGenerator::js_define_method("Rho:System:localServerPort=", js_System_setLocalServerPort);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_localServerPort=", js_s_System_def_setLocalServerPort);;

    rho::apiGenerator::js_define_method("Rho:System:freeServerPort", js_System_getFreeServerPort);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_freeServerPort", js_s_System_def_getFreeServerPort);;

    rho::apiGenerator::js_define_method("Rho:System:screenAutoRotate", js_System_getScreenAutoRotate);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_screenAutoRotate", js_s_System_def_getScreenAutoRotate);;

    rho::apiGenerator::js_define_method("Rho:System:screenAutoRotate=", js_System_setScreenAutoRotate);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_screenAutoRotate=", js_s_System_def_setScreenAutoRotate);;

    rho::apiGenerator::js_define_method("Rho:System:hasTouchscreen", js_System_getHasTouchscreen);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_hasTouchscreen", js_s_System_def_getHasTouchscreen);;

    rho::apiGenerator::js_define_method("Rho:System:securityTokenNotPassed", js_System_getSecurityTokenNotPassed);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_securityTokenNotPassed", js_s_System_def_getSecurityTokenNotPassed);;

    rho::apiGenerator::js_define_method("Rho:System:applicationInstall", js_System_applicationInstall);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_applicationInstall", js_s_System_def_applicationInstall);;

    rho::apiGenerator::js_define_method("Rho:System:isApplicationInstalled", js_System_isApplicationInstalled);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_isApplicationInstalled", js_s_System_def_isApplicationInstalled);;

    rho::apiGenerator::js_define_method("Rho:System:applicationUninstall", js_System_applicationUninstall);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_applicationUninstall", js_s_System_def_applicationUninstall);;

    rho::apiGenerator::js_define_method("Rho:System:getStartParams", js_System_getStartParams);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getStartParams", js_s_System_def_getStartParams);;

    rho::apiGenerator::js_define_method("Rho:System:openUrl", js_System_openUrl);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_openUrl", js_s_System_def_openUrl);;

    rho::apiGenerator::js_define_method("Rho:System:runApplication", js_System_runApplication);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_runApplication", js_s_System_def_runApplication);;

    rho::apiGenerator::js_define_method("Rho:System:unzipFile", js_System_unzipFile);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_unzipFile", js_s_System_def_unzipFile);;

    rho::apiGenerator::js_define_method("Rho:System:zipFile", js_System_zipFile);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_zipFile", js_s_System_def_zipFile);;

    rho::apiGenerator::js_define_method("Rho:System:zipFiles", js_System_zipFiles);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_zipFiles", js_s_System_def_zipFiles);;

    rho::apiGenerator::js_define_method("Rho:System:setRegistrySetting", js_System_setRegistrySetting);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setRegistrySetting", js_s_System_def_setRegistrySetting);;

    rho::apiGenerator::js_define_method("Rho:System:getRegistrySetting", js_System_getRegistrySetting);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getRegistrySetting", js_s_System_def_getRegistrySetting);;

    rho::apiGenerator::js_define_method("Rho:System:setWindowFrame", js_System_setWindowFrame);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setWindowFrame", js_s_System_def_setWindowFrame);;

    rho::apiGenerator::js_define_method("Rho:System:setWindowPosition", js_System_setWindowPosition);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setWindowPosition", js_s_System_def_setWindowPosition);;

    rho::apiGenerator::js_define_method("Rho:System:getProperty", js_System_getProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getProperty", js_s_System_def_getProperty);;

    rho::apiGenerator::js_define_method("Rho:System:getProperties", js_System_getProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getProperties", js_s_System_def_getProperties);;

    rho::apiGenerator::js_define_method("Rho:System:getAllProperties", js_System_getAllProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getAllProperties", js_s_System_def_getAllProperties);;

    rho::apiGenerator::js_define_method("Rho:System:setProperty", js_System_setProperty);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setProperty", js_s_System_def_setProperty);;

    rho::apiGenerator::js_define_method("Rho:System:setProperties", js_System_setProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setProperties", js_s_System_def_setProperties);;

    rho::apiGenerator::js_define_method("Rho:System:clearAllProperties", js_System_clearAllProperties);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_clearAllProperties", js_s_System_def_clearAllProperties);;


    rho::apiGenerator::js_define_method("Rho:System:getDefaultID", js_s_System_getDefaultID);
    rho::apiGenerator::js_define_method("Rho:System:setDefaultID", js_s_System_setDefaultID);

}


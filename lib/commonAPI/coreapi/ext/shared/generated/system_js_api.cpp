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

rho::String js_System_getDeviceId(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getDeviceId(rho::json::CJSONArray& argv, const rho::String& strObjID);

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

rho::String js_System_getScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_getScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_setScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_setScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_System_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);
rho::String js_s_System_def_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

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

    rho::apiGenerator::js_define_method("Rho:System:getPlatform", js_System_getPlatform);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getPlatform", js_s_System_def_getPlatform);;

    rho::apiGenerator::js_define_method("Rho:System:getHasCamera", js_System_getHasCamera);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getHasCamera", js_s_System_def_getHasCamera);;

    rho::apiGenerator::js_define_method("Rho:System:getScreenWidth", js_System_getScreenWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getScreenWidth", js_s_System_def_getScreenWidth);;

    rho::apiGenerator::js_define_method("Rho:System:getScreenHeight", js_System_getScreenHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getScreenHeight", js_s_System_def_getScreenHeight);;

    rho::apiGenerator::js_define_method("Rho:System:getRealScreenWidth", js_System_getRealScreenWidth);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getRealScreenWidth", js_s_System_def_getRealScreenWidth);;

    rho::apiGenerator::js_define_method("Rho:System:getRealScreenHeight", js_System_getRealScreenHeight);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getRealScreenHeight", js_s_System_def_getRealScreenHeight);;

    rho::apiGenerator::js_define_method("Rho:System:getScreenOrientation", js_System_getScreenOrientation);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getScreenOrientation", js_s_System_def_getScreenOrientation);;

    rho::apiGenerator::js_define_method("Rho:System:getPpiX", js_System_getPpiX);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getPpiX", js_s_System_def_getPpiX);;

    rho::apiGenerator::js_define_method("Rho:System:getPpiY", js_System_getPpiY);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getPpiY", js_s_System_def_getPpiY);;

    rho::apiGenerator::js_define_method("Rho:System:getPhoneNumber", js_System_getPhoneNumber);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getPhoneNumber", js_s_System_def_getPhoneNumber);;

    rho::apiGenerator::js_define_method("Rho:System:getDeviceOwnerEmail", js_System_getDeviceOwnerEmail);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getDeviceOwnerEmail", js_s_System_def_getDeviceOwnerEmail);;

    rho::apiGenerator::js_define_method("Rho:System:getDeviceOwnerName", js_System_getDeviceOwnerName);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getDeviceOwnerName", js_s_System_def_getDeviceOwnerName);;

    rho::apiGenerator::js_define_method("Rho:System:getDeviceId", js_System_getDeviceId);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getDeviceId", js_s_System_def_getDeviceId);;

    rho::apiGenerator::js_define_method("Rho:System:getPhoneId", js_System_getPhoneId);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getPhoneId", js_s_System_def_getPhoneId);;

    rho::apiGenerator::js_define_method("Rho:System:getDeviceName", js_System_getDeviceName);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getDeviceName", js_s_System_def_getDeviceName);;

    rho::apiGenerator::js_define_method("Rho:System:getOsVersion", js_System_getOsVersion);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getOsVersion", js_s_System_def_getOsVersion);;

    rho::apiGenerator::js_define_method("Rho:System:getLocale", js_System_getLocale);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getLocale", js_s_System_def_getLocale);;

    rho::apiGenerator::js_define_method("Rho:System:getCountry", js_System_getCountry);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getCountry", js_s_System_def_getCountry);;

    rho::apiGenerator::js_define_method("Rho:System:getIsEmulator", js_System_getIsEmulator);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getIsEmulator", js_s_System_def_getIsEmulator);;

    rho::apiGenerator::js_define_method("Rho:System:getHasCalendar", js_System_getHasCalendar);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getHasCalendar", js_s_System_def_getHasCalendar);;

    rho::apiGenerator::js_define_method("Rho:System:getIsMotorolaDevice", js_System_getIsMotorolaDevice);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getIsMotorolaDevice", js_s_System_def_getIsMotorolaDevice);;

    rho::apiGenerator::js_define_method("Rho:System:getOemInfo", js_System_getOemInfo);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getOemInfo", js_s_System_def_getOemInfo);;

    rho::apiGenerator::js_define_method("Rho:System:getUuid", js_System_getUuid);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getUuid", js_s_System_def_getUuid);;

    rho::apiGenerator::js_define_method("Rho:System:getApplicationIconBadge", js_System_getApplicationIconBadge);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getApplicationIconBadge", js_s_System_def_getApplicationIconBadge);;

    rho::apiGenerator::js_define_method("Rho:System:setApplicationIconBadge", js_System_setApplicationIconBadge);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setApplicationIconBadge", js_s_System_def_setApplicationIconBadge);;

    rho::apiGenerator::js_define_method("Rho:System:getHttpProxyURI", js_System_getHttpProxyURI);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getHttpProxyURI", js_s_System_def_getHttpProxyURI);;

    rho::apiGenerator::js_define_method("Rho:System:setHttpProxyURI", js_System_setHttpProxyURI);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setHttpProxyURI", js_s_System_def_setHttpProxyURI);;

    rho::apiGenerator::js_define_method("Rho:System:getLockWindowSize", js_System_getLockWindowSize);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getLockWindowSize", js_s_System_def_getLockWindowSize);;

    rho::apiGenerator::js_define_method("Rho:System:setLockWindowSize", js_System_setLockWindowSize);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setLockWindowSize", js_s_System_def_setLockWindowSize);;

    rho::apiGenerator::js_define_method("Rho:System:getShowKeyboard", js_System_getShowKeyboard);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getShowKeyboard", js_s_System_def_getShowKeyboard);;

    rho::apiGenerator::js_define_method("Rho:System:setShowKeyboard", js_System_setShowKeyboard);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setShowKeyboard", js_s_System_def_setShowKeyboard);;

    rho::apiGenerator::js_define_method("Rho:System:getFullScreen", js_System_getFullScreen);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getFullScreen", js_s_System_def_getFullScreen);;

    rho::apiGenerator::js_define_method("Rho:System:setFullScreen", js_System_setFullScreen);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setFullScreen", js_s_System_def_setFullScreen);;

    rho::apiGenerator::js_define_method("Rho:System:getLocalServerPort", js_System_getLocalServerPort);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getLocalServerPort", js_s_System_def_getLocalServerPort);;

    rho::apiGenerator::js_define_method("Rho:System:setLocalServerPort", js_System_setLocalServerPort);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setLocalServerPort", js_s_System_def_setLocalServerPort);;

    rho::apiGenerator::js_define_method("Rho:System:getScreenAutoRotate", js_System_getScreenAutoRotate);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_getScreenAutoRotate", js_s_System_def_getScreenAutoRotate);;

    rho::apiGenerator::js_define_method("Rho:System:setScreenAutoRotate", js_System_setScreenAutoRotate);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_setScreenAutoRotate", js_s_System_def_setScreenAutoRotate);;

    rho::apiGenerator::js_define_method("Rho:System:applicationInstall", js_System_applicationInstall);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_applicationInstall", js_s_System_def_applicationInstall);;

    rho::apiGenerator::js_define_method("Rho:System:isApplicationInstalled", js_System_isApplicationInstalled);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_isApplicationInstalled", js_s_System_def_isApplicationInstalled);;

    rho::apiGenerator::js_define_method("Rho:System:applicationUninstall", js_System_applicationUninstall);
    //  should define static method !     rho::apiGenerator::js_define_method("Rho:System:def_applicationUninstall", js_s_System_def_applicationUninstall);;

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


#include "api_generator/js_helpers.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "System"


rho::String js_s_System_getPlatform(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasCamera(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getRealScreenWidth(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getRealScreenHeight(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getScreenOrientation(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getPpiX(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getPpiY(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getPhoneNumber(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getDeviceOwnerEmail(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getDeviceOwnerName(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getDevicePushId(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getPhoneId(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getDeviceName(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getOsVersion(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getLocale(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getCountry(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getIsEmulator(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasCalendar(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getIsMotorolaDevice(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getOemInfo(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getUuid(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setApplicationIconBadge(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setHttpProxyURI(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setLockWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setShowKeyboard(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getFullScreen(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setFullScreen(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getLocalServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setLocalServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getFreeServerPort(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setScreenAutoRotate(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasTouchscreen(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getSecurityTokenNotPassed(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getWebviewFramework(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getScreenSleeping(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setScreenSleeping(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasNetwork(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasWifiNetwork(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasCellNetwork(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getHasSqlite(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_applicationInstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_isApplicationInstalled(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_applicationUninstall(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getStartParams(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_openUrl(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_unzipFile(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_zipFile(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_zipFiles(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getRegistrySetting(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setWindowFrame(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setWindowPosition(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setWindowSize(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_bringToFront(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_replaceCurrentBundle(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_deleteFolder(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setDoNotBackupAttribute(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_isBlobAttr(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_updateBlobAttribs(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_get_locale(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_set_locale(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setPushNotification(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setScreenRotationNotification(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_exit(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_set_sleeping(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_startTimer(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_stopTimer(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setNetworkStatusNotify(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_clearNetworkStatusNotify(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_set_http_proxy_url(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_unset_http_proxy(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_set_application_icon_badge(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_runApplication(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_getAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setProperty(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_setProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);

rho::String js_s_System_clearAllProperties(rho::json::CJSONArray& argv, const rho::String& strObjID);





extern "C" void Init_JSAPI_System(void)
{

    rho::apiGenerator::js_define_method("Rho:System:platform", js_s_System_getPlatform);

    rho::apiGenerator::js_define_method("Rho:System:hasCamera", js_s_System_getHasCamera);

    rho::apiGenerator::js_define_method("Rho:System:screenWidth", js_s_System_getScreenWidth);

    rho::apiGenerator::js_define_method("Rho:System:screenHeight", js_s_System_getScreenHeight);

    rho::apiGenerator::js_define_method("Rho:System:realScreenWidth", js_s_System_getRealScreenWidth);

    rho::apiGenerator::js_define_method("Rho:System:realScreenHeight", js_s_System_getRealScreenHeight);

    rho::apiGenerator::js_define_method("Rho:System:screenOrientation", js_s_System_getScreenOrientation);

    rho::apiGenerator::js_define_method("Rho:System:ppiX", js_s_System_getPpiX);

    rho::apiGenerator::js_define_method("Rho:System:ppiY", js_s_System_getPpiY);

    rho::apiGenerator::js_define_method("Rho:System:phoneNumber", js_s_System_getPhoneNumber);

    rho::apiGenerator::js_define_method("Rho:System:deviceOwnerEmail", js_s_System_getDeviceOwnerEmail);

    rho::apiGenerator::js_define_method("Rho:System:deviceOwnerName", js_s_System_getDeviceOwnerName);

    rho::apiGenerator::js_define_method("Rho:System:devicePushId", js_s_System_getDevicePushId);

    rho::apiGenerator::js_define_method("Rho:System:phoneId", js_s_System_getPhoneId);

    rho::apiGenerator::js_define_method("Rho:System:deviceName", js_s_System_getDeviceName);

    rho::apiGenerator::js_define_method("Rho:System:osVersion", js_s_System_getOsVersion);

    rho::apiGenerator::js_define_method("Rho:System:locale", js_s_System_getLocale);

    rho::apiGenerator::js_define_method("Rho:System:country", js_s_System_getCountry);

    rho::apiGenerator::js_define_method("Rho:System:isEmulator", js_s_System_getIsEmulator);

    rho::apiGenerator::js_define_method("Rho:System:hasCalendar", js_s_System_getHasCalendar);

    rho::apiGenerator::js_define_method("Rho:System:isMotorolaDevice", js_s_System_getIsMotorolaDevice);

    rho::apiGenerator::js_define_method("Rho:System:oemInfo", js_s_System_getOemInfo);

    rho::apiGenerator::js_define_method("Rho:System:uuid", js_s_System_getUuid);

    rho::apiGenerator::js_define_method("Rho:System:applicationIconBadge", js_s_System_getApplicationIconBadge);

    rho::apiGenerator::js_define_method("Rho:System:applicationIconBadge=", js_s_System_setApplicationIconBadge);

    rho::apiGenerator::js_define_method("Rho:System:httpProxyURI", js_s_System_getHttpProxyURI);

    rho::apiGenerator::js_define_method("Rho:System:httpProxyURI=", js_s_System_setHttpProxyURI);

    rho::apiGenerator::js_define_method("Rho:System:lockWindowSize", js_s_System_getLockWindowSize);

    rho::apiGenerator::js_define_method("Rho:System:lockWindowSize=", js_s_System_setLockWindowSize);

    rho::apiGenerator::js_define_method("Rho:System:showKeyboard", js_s_System_getShowKeyboard);

    rho::apiGenerator::js_define_method("Rho:System:showKeyboard=", js_s_System_setShowKeyboard);

    rho::apiGenerator::js_define_method("Rho:System:fullScreen", js_s_System_getFullScreen);

    rho::apiGenerator::js_define_method("Rho:System:fullScreen=", js_s_System_setFullScreen);

    rho::apiGenerator::js_define_method("Rho:System:localServerPort", js_s_System_getLocalServerPort);

    rho::apiGenerator::js_define_method("Rho:System:localServerPort=", js_s_System_setLocalServerPort);

    rho::apiGenerator::js_define_method("Rho:System:freeServerPort", js_s_System_getFreeServerPort);

    rho::apiGenerator::js_define_method("Rho:System:screenAutoRotate", js_s_System_getScreenAutoRotate);

    rho::apiGenerator::js_define_method("Rho:System:screenAutoRotate=", js_s_System_setScreenAutoRotate);

    rho::apiGenerator::js_define_method("Rho:System:hasTouchscreen", js_s_System_getHasTouchscreen);

    rho::apiGenerator::js_define_method("Rho:System:securityTokenNotPassed", js_s_System_getSecurityTokenNotPassed);

    rho::apiGenerator::js_define_method("Rho:System:webviewFramework", js_s_System_getWebviewFramework);

    rho::apiGenerator::js_define_method("Rho:System:screenSleeping", js_s_System_getScreenSleeping);

    rho::apiGenerator::js_define_method("Rho:System:screenSleeping=", js_s_System_setScreenSleeping);

    rho::apiGenerator::js_define_method("Rho:System:hasNetwork", js_s_System_getHasNetwork);

    rho::apiGenerator::js_define_method("Rho:System:hasWifiNetwork", js_s_System_getHasWifiNetwork);

    rho::apiGenerator::js_define_method("Rho:System:hasCellNetwork", js_s_System_getHasCellNetwork);

    rho::apiGenerator::js_define_method("Rho:System:hasSqlite", js_s_System_getHasSqlite);

    rho::apiGenerator::js_define_method("Rho:System:applicationInstall", js_s_System_applicationInstall);

    rho::apiGenerator::js_define_method("Rho:System:isApplicationInstalled", js_s_System_isApplicationInstalled);

    rho::apiGenerator::js_define_method("Rho:System:applicationUninstall", js_s_System_applicationUninstall);

    rho::apiGenerator::js_define_method("Rho:System:getStartParams", js_s_System_getStartParams);

    rho::apiGenerator::js_define_method("Rho:System:openUrl", js_s_System_openUrl);

    rho::apiGenerator::js_define_method("Rho:System:unzipFile", js_s_System_unzipFile);

    rho::apiGenerator::js_define_method("Rho:System:zipFile", js_s_System_zipFile);

    rho::apiGenerator::js_define_method("Rho:System:zipFiles", js_s_System_zipFiles);

    rho::apiGenerator::js_define_method("Rho:System:setRegistrySetting", js_s_System_setRegistrySetting);

    rho::apiGenerator::js_define_method("Rho:System:getRegistrySetting", js_s_System_getRegistrySetting);

    rho::apiGenerator::js_define_method("Rho:System:setWindowFrame", js_s_System_setWindowFrame);

    rho::apiGenerator::js_define_method("Rho:System:setWindowPosition", js_s_System_setWindowPosition);

    rho::apiGenerator::js_define_method("Rho:System:setWindowSize", js_s_System_setWindowSize);

    rho::apiGenerator::js_define_method("Rho:System:bringToFront", js_s_System_bringToFront);

    rho::apiGenerator::js_define_method("Rho:System:replaceCurrentBundle", js_s_System_replaceCurrentBundle);

    rho::apiGenerator::js_define_method("Rho:System:deleteFolder", js_s_System_deleteFolder);

    rho::apiGenerator::js_define_method("Rho:System:setDoNotBackupAttribute", js_s_System_setDoNotBackupAttribute);

    rho::apiGenerator::js_define_method("Rho:System:isBlobAttr", js_s_System_isBlobAttr);

    rho::apiGenerator::js_define_method("Rho:System:updateBlobAttribs", js_s_System_updateBlobAttribs);

    rho::apiGenerator::js_define_method("Rho:System:get_locale", js_s_System_get_locale);

    rho::apiGenerator::js_define_method("Rho:System:set_locale", js_s_System_set_locale);

    rho::apiGenerator::js_define_method("Rho:System:setPushNotification", js_s_System_setPushNotification);

    rho::apiGenerator::js_define_method("Rho:System:setScreenRotationNotification", js_s_System_setScreenRotationNotification);

    rho::apiGenerator::js_define_method("Rho:System:exit", js_s_System_exit);

    rho::apiGenerator::js_define_method("Rho:System:set_sleeping", js_s_System_set_sleeping);

    rho::apiGenerator::js_define_method("Rho:System:startTimer", js_s_System_startTimer);

    rho::apiGenerator::js_define_method("Rho:System:stopTimer", js_s_System_stopTimer);

    rho::apiGenerator::js_define_method("Rho:System:setNetworkStatusNotify", js_s_System_setNetworkStatusNotify);

    rho::apiGenerator::js_define_method("Rho:System:clearNetworkStatusNotify", js_s_System_clearNetworkStatusNotify);

    rho::apiGenerator::js_define_method("Rho:System:set_http_proxy_url", js_s_System_set_http_proxy_url);

    rho::apiGenerator::js_define_method("Rho:System:unset_http_proxy", js_s_System_unset_http_proxy);

    rho::apiGenerator::js_define_method("Rho:System:set_application_icon_badge", js_s_System_set_application_icon_badge);

    rho::apiGenerator::js_define_method("Rho:System:runApplication", js_s_System_runApplication);

    rho::apiGenerator::js_define_method("Rho:System:getProperty", js_s_System_getProperty);

    rho::apiGenerator::js_define_method("Rho:System:getProperties", js_s_System_getProperties);

    rho::apiGenerator::js_define_method("Rho:System:getAllProperties", js_s_System_getAllProperties);

    rho::apiGenerator::js_define_method("Rho:System:setProperty", js_s_System_setProperty);

    rho::apiGenerator::js_define_method("Rho:System:setProperties", js_s_System_setProperties);

    rho::apiGenerator::js_define_method("Rho:System:clearAllProperties", js_s_System_clearAllProperties);


}


#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
///////////////////////////////////////////////////////////
struct ISystem
{
//constants


//methods
    virtual ~ISystem(){}


};

struct ISystemSingleton
{
//constants

    static const char PLATFORM_WM_CE[];// "WINDOWS" 
    static const char PLATFORM_ANDROID[];// "ANDROID" 
    static const char PLATFORM_IOS[];// "APPLE" 
    static const char PLATFORM_WP8[];// "WP8" 
    static const char PLATFORM_WINDOWS_DESKTOP[];// "WINDOWS_DESKTOP" 
    static const char SCREEN_PORTRAIT[];// "portrait" 
    static const char SCREEN_LANDSCAPE[];// "landscape" 

    virtual ~ISystemSingleton(){}

//methods
    virtual void getPlatform(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasCamera(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenWidth(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenHeight(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getRealScreenWidth(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getRealScreenHeight(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenOrientation(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPpiX(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPpiY(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPhoneNumber(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getDeviceOwnerEmail(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getDeviceOwnerName(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getDevicePushId(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getPhoneId(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getDeviceName(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getOsVersion(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getLocale(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getCountry(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getIsEmulator(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasCalendar(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getIsMotorolaDevice(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getOemInfo(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getUuid(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getApplicationIconBadge(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setApplicationIconBadge( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHttpProxyURI(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setHttpProxyURI( const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getLockWindowSize(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setLockWindowSize( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getShowKeyboard(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setShowKeyboard( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getFullScreen(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setFullScreen( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getLocalServerPort(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setLocalServerPort( int value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getFreeServerPort(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenAutoRotate(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setScreenAutoRotate( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getSecurityTokenNotPassed(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasNetwork(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasWifiNetwork(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasCellNetwork(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasSqlite(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void applicationInstall( const rho::String& applicationUrl, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isApplicationInstalled( const rho::String& applicationName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void applicationUninstall( const rho::String& applicationName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getStartParams(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void openUrl( const rho::String& url, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void unzipFile( const rho::String& localPathToZip,  const rho::String& password, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void zipFile( const rho::String& localPathToZip,  const rho::String& localPathToFile,  const rho::String& password, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void zipFiles( const rho::String& localPathToZip,  const rho::String& basePath,  const rho::Vector<rho::String>& filePathsToZip,  const rho::String& password, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setRegistrySetting( int hive,  int type,  const rho::String& subKey,  const rho::String& setting,  const rho::String& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getRegistrySetting( int hive,  const rho::String& subKey,  const rho::String& setting, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setWindowFrame( int x,  int y,  int width,  int height, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setWindowPosition( int x,  int y, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setWindowSize( int width,  int height, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void replaceCurrentBundle( const rho::String& pathToBundle,  const rho::Hashtable<rho::String, rho::String>& params, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void deleteFolder( const rho::String& pathToFolder, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setDoNotBackupAttribute( const rho::String& pathToFile, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isBlobAttr( const rho::String& partition,  int sourceID,  const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void updateBlobAttribs( const rho::String& partition,  int sourceID, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void get_locale(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setPushNotification( const rho::String& url,  const rho::String& url_params,  const rho::String& push_client, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setScreenRotationNotification( const rho::String& url,  const rho::String& url_params, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void exit(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void set_sleeping( bool enable, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void startTimer( int interval,  const rho::String& url,  const rho::String& url_params, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void stopTimer( const rho::String& url, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setNetworkStatusNotify( const rho::String& url,  int poll_interval, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void clearNetworkStatusNotify(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void unset_http_proxy(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void set_application_icon_badge( int badgeNumber, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void runApplication( const rho::String& appName,  const rho::String& params,  bool blockingCall, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::String& propertyName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::String>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::String& propertyName,  const rho::String& propertyValue, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::String, rho::String>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void clearAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;


    virtual void addCommandToQueue(rho::common::CInstanceClassFunctorBase<rho::apiGenerator::CMethodResult>* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct ISystemFactory
{
    virtual ~ISystemFactory(){}

    virtual ISystemSingleton* getModuleSingleton() = 0;


};


}

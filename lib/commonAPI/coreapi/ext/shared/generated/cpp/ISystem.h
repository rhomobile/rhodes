#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"


namespace rho {
///////////////////////////////////////////////////////////
struct ISystem
{
//constants

    static const wchar_t PLATFORM_WM_CE[];// L"WINDOWS" 
    static const wchar_t PLATFORM_ANDROID[];// L"ANDROID" 
    static const wchar_t PLATFORM_IOS[];// L"APPLE" 
    static const wchar_t PLATFORM_WP8[];// L"WP8" 
    static const wchar_t PLATFORM_WINDOWS_DESKTOP[];// L"WINDOWS_DESKTOP" 
    static const wchar_t SCREEN_PORTRAIT[];// L"portrait" 
    static const wchar_t SCREEN_LANDSCAPE[];// L"landscape" 

//methods
    virtual ~ISystem(){}

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
    virtual void setApplicationIconBadge( int64 value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHttpProxyURI(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setHttpProxyURI( const rho::StringW& value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getLockWindowSize(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setLockWindowSize( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getShowKeyboard(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setShowKeyboard( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getFullScreen(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setFullScreen( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getLocalServerPort(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setLocalServerPort( int64 value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getFreeServerPort(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenAutoRotate(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setScreenAutoRotate( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getSecurityTokenNotPassed(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getWebviewFramework(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getScreenSleeping(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setScreenSleeping( bool value, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void applicationInstall( const rho::StringW& applicationUrl, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isApplicationInstalled( const rho::StringW& applicationName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void applicationUninstall( const rho::StringW& applicationName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getStartParams(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void openUrl( const rho::StringW& url, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void unzipFile( const rho::StringW& localPathToZip,  const rho::StringW& password, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void zipFile( const rho::StringW& localPathToZip,  const rho::StringW& localPathToFile,  const rho::StringW& password, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void zipFiles( const rho::StringW& localPathToZip,  const rho::StringW& basePath,  const rho::Vector<rho::StringW>& filePathsToZip,  const rho::StringW& password, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setRegistrySetting( const rho::StringW& keyPath,  const rho::StringW& keyValue, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getRegistrySetting( const rho::StringW& keyPath, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setWindowFrame( int64 x,  int64 y,  int64 width,  int64 height, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setWindowPosition( int64 x,  int64 y, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setWindowSize( int64 width,  int64 height, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void bringToFront(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void replaceCurrentBundle( const rho::StringW& pathToBundle, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void deleteFolder( const rho::StringW& pathToFolder, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setDoNotBackupAttribute( const rho::StringW& pathToFile, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void isBlobAttr( const rho::StringW& partition,  int64 sourceID,  const rho::StringW& attrName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void updateBlobAttribs( const rho::StringW& partition,  int64 sourceID, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::StringW& propertyName, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::StringW>& arrayofNames, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void getAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, rho::apiGenerator::CMethodResult& oResult) = 0;
    virtual void clearAllProperties(rho::apiGenerator::CMethodResult& oResult) = 0;

};

struct ISystemSingleton
{
    virtual ~ISystemSingleton(){}



    virtual rho::StringW getDefaultID() = 0;
    virtual rho::StringW getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::StringW& strID) = 0;

    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
    virtual void callCommandInThread(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct ISystemFactory
{
    virtual ~ISystemFactory(){}

    virtual ISystemSingleton* getModuleSingleton() = 0;


    virtual ISystem* getModuleByID(const rho::StringW& strID) = 0;

};


}

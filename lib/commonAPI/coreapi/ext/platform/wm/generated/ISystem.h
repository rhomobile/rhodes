#pragma once

#include "common/RhoStd.h"
#include "api_generator/MethodResult.h"
#include "api_generator/BaseClasses.h"

///////////////////////////////////////////////////////////
struct ISystem
{
    virtual ~ISystem(){}

    virtual void getPlatform(CMethodResult& oResult) = 0;
    virtual void getHasCamera(CMethodResult& oResult) = 0;
    virtual void getScreenWidth(CMethodResult& oResult) = 0;
    virtual void getScreenHeight(CMethodResult& oResult) = 0;
    virtual void getRealScreenWidth(CMethodResult& oResult) = 0;
    virtual void getRealScreenHeight(CMethodResult& oResult) = 0;
    virtual void getScreenOrientation(CMethodResult& oResult) = 0;
    virtual void getPpiX(CMethodResult& oResult) = 0;
    virtual void getPpiY(CMethodResult& oResult) = 0;
    virtual void getPhoneNumber(CMethodResult& oResult) = 0;
    virtual void getDeviceOwnerEmail(CMethodResult& oResult) = 0;
    virtual void getDeviceOwnerName(CMethodResult& oResult) = 0;
    virtual void getDeviceId(CMethodResult& oResult) = 0;
    virtual void getPhoneId(CMethodResult& oResult) = 0;
    virtual void getDeviceName(CMethodResult& oResult) = 0;
    virtual void getOsVersion(CMethodResult& oResult) = 0;
    virtual void getLocale(CMethodResult& oResult) = 0;
    virtual void getCountry(CMethodResult& oResult) = 0;
    virtual void getIsEmulator(CMethodResult& oResult) = 0;
    virtual void getHasCalendar(CMethodResult& oResult) = 0;
    virtual void getIsMotorolaDevice(CMethodResult& oResult) = 0;
    virtual void getOemInfo(CMethodResult& oResult) = 0;
    virtual void getUuid(CMethodResult& oResult) = 0;
    virtual void getApplicationIconBadge(CMethodResult& oResult) = 0;
    virtual void setApplicationIconBadge( int value, CMethodResult& oResult) = 0;
    virtual void getHttpProxyURI(CMethodResult& oResult) = 0;
    virtual void setHttpProxyURI( const rho::StringW& value, CMethodResult& oResult) = 0;
    virtual void getLockWindowSize(CMethodResult& oResult) = 0;
    virtual void setLockWindowSize( bool value, CMethodResult& oResult) = 0;
    virtual void getShowKeyboard(CMethodResult& oResult) = 0;
    virtual void setShowKeyboard( bool value, CMethodResult& oResult) = 0;
    virtual void applicationInstall( const rho::StringW& applicationUrl, CMethodResult& oResult) = 0;
    virtual void isApplicationInstalled( const rho::StringW& applicationName, CMethodResult& oResult) = 0;
    virtual void applicationUninstall( const rho::StringW& applicationName, CMethodResult& oResult) = 0;
    virtual void openUrl( const rho::StringW& url, CMethodResult& oResult) = 0;
    virtual void runApplication( const rho::StringW& appName,  const rho::StringW& params,  bool blockingCall, CMethodResult& oResult) = 0;
    virtual void unzipFile( const rho::StringW& localPathToZip, CMethodResult& oResult) = 0;
    virtual void zipFile( const rho::StringW& localPathToZip,  const rho::StringW& localPathToFile,  const rho::StringW& password, CMethodResult& oResult) = 0;
    virtual void zipFiles( const rho::StringW& localPathToZip,  const rho::StringW& basePath,  const rho::Vector<rho::StringW>& filePathsToZip,  const rho::StringW& password, CMethodResult& oResult) = 0;
    virtual void setRegistrySetting( const rho::StringW& keyPath,  const rho::StringW& keyValue, CMethodResult& oResult) = 0;
    virtual void getRegistrySetting( const rho::StringW& keyPath, CMethodResult& oResult) = 0;
    virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult) = 0;
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult) = 0;
    virtual void getProperty( const rho::StringW& propertyName, CMethodResult& oResult) = 0;
    virtual void getProperties( const rho::Vector<rho::StringW>& arrayofNames, CMethodResult& oResult) = 0;
    virtual void getAllProperties(CMethodResult& oResult) = 0;
    virtual void setProperty( const rho::StringW& propertyName,  const rho::StringW& propertyValue, CMethodResult& oResult) = 0;
    virtual void setProperties( const rho::Hashtable<rho::StringW, rho::StringW>& propertyMap, CMethodResult& oResult) = 0;
    virtual void clearAllProperties(CMethodResult& oResult) = 0;

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

class CSystemFactoryBase : public CModuleFactoryBase<ISystem, ISystemSingleton, ISystemFactory>
{
protected:
    static rho::common::CAutoPtr<CSystemFactoryBase> m_pInstance;

public:

    static void setInstance(CSystemFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CSystemFactoryBase* getInstance(){ return m_pInstance; }

    static ISystemSingleton* getSystemSingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_System_API();

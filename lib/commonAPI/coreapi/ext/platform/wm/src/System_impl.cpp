#include "../generated/ISystem.h"

namespace rho {

using namespace apiGenerator;

class CSystemImpl: public CModuleBase<ISystem>
{
public:
    CSystemImpl(const rho::StringW& strID): CModuleBase<ISystem>(strID)
    {
    }

    virtual void getPlatform(CMethodResult& oResult){}
    virtual void getHasCamera(CMethodResult& oResult){}
    virtual void getScreenWidth(CMethodResult& oResult){}
    virtual void getScreenHeight(CMethodResult& oResult){}
    virtual void getRealScreenWidth(CMethodResult& oResult){}
    virtual void getRealScreenHeight(CMethodResult& oResult){}
    virtual void getScreenOrientation(CMethodResult& oResult){}
    virtual void getPpiX(CMethodResult& oResult){}
    virtual void getPpiY(CMethodResult& oResult){}
    virtual void getPhoneNumber(CMethodResult& oResult){}
    virtual void getDeviceOwnerEmail(CMethodResult& oResult){}
    virtual void getDeviceOwnerName(CMethodResult& oResult){}
    virtual void getDeviceId(CMethodResult& oResult){}
    virtual void getPhoneId(CMethodResult& oResult){}
    virtual void getDeviceName(CMethodResult& oResult){}
    virtual void getOsVersion(CMethodResult& oResult){}
    virtual void getLocale(CMethodResult& oResult){}
    virtual void getCountry(CMethodResult& oResult){}
    virtual void getIsEmulator(CMethodResult& oResult){}
    virtual void getHasCalendar(CMethodResult& oResult){}
    virtual void getIsMotorolaDevice(CMethodResult& oResult){}
    virtual void getOemInfo(CMethodResult& oResult){}
    virtual void getUuid(CMethodResult& oResult){}
    virtual void getApplicationIconBadge(CMethodResult& oResult){}
    virtual void setApplicationIconBadge( int value, CMethodResult& oResult){}
    virtual void getHttpProxyURI(CMethodResult& oResult){}
    virtual void setHttpProxyURI( const rho::StringW& value, CMethodResult& oResult){}
    virtual void getLockWindowSize(CMethodResult& oResult){}
    virtual void setLockWindowSize( bool value, CMethodResult& oResult){}
    virtual void getShowKeyboard(CMethodResult& oResult){}
    virtual void setShowKeyboard( bool value, CMethodResult& oResult){}
    virtual void getFullScreen(CMethodResult& oResult){}
    virtual void setFullScreen( bool value, CMethodResult& oResult){}
    virtual void getLocalServerPort(CMethodResult& oResult){}
    virtual void setLocalServerPort( int value, CMethodResult& oResult){}
    virtual void getScreenAutoRotate(CMethodResult& oResult){}
    virtual void setScreenAutoRotate( bool value, CMethodResult& oResult){}

    virtual void applicationInstall( const rho::StringW& applicationUrl, CMethodResult& oResult){}
    virtual void isApplicationInstalled( const rho::StringW& applicationName, CMethodResult& oResult){}
    virtual void applicationUninstall( const rho::StringW& applicationName, CMethodResult& oResult){}
    virtual void openUrl( const rho::StringW& url, CMethodResult& oResult){}
    virtual void runApplication( const rho::StringW& appName,  const rho::StringW& params,  bool blockingCall, CMethodResult& oResult){}
    virtual void unzipFile( const rho::StringW& localPathToZip, CMethodResult& oResult){}
    virtual void zipFile( const rho::StringW& localPathToZip,  const rho::StringW& localPathToFile,  const rho::StringW& password, CMethodResult& oResult){}
    virtual void zipFiles( const rho::StringW& localPathToZip,  const rho::StringW& basePath,  const rho::Vector<rho::StringW>& filePathsToZip,  const rho::StringW& password, CMethodResult& oResult){}
    virtual void setRegistrySetting( const rho::StringW& keyPath,  const rho::StringW& keyValue, CMethodResult& oResult){}
    virtual void getRegistrySetting( const rho::StringW& keyPath, CMethodResult& oResult){}
    virtual void setWindowFrame( int x,  int y,  int width,  int height, CMethodResult& oResult){}
    virtual void setWindowPosition( int x,  int y, CMethodResult& oResult){}

};

class CSystemSingleton: public CModuleSingletonBase<ISystemSingleton>
{
    ~CSystemSingleton(){}

    virtual rho::StringW getInitialDefaultID();
};

class CSystemFactory: public CSystemFactoryBase
{
    ~CSystemFactory(){}
    virtual ISystemSingleton* createModuleSingleton();
    virtual ISystem* createModuleByID(const rho::StringW& strID);
};

extern "C" void Init_System_extension()
{
    CSystemFactory::setInstance( new CSystemFactory() );
    Init_System_API();
}

ISystemSingleton* CSystemFactory::createModuleSingleton()
{
    return new CSystemSingleton();
}

ISystem* CSystemFactory::createModuleByID(const rho::StringW& strID)
{
    return new CSystemImpl(strID);
}

rho::StringW CSystemSingleton::getInitialDefaultID()
{
    return L"1";
}

}
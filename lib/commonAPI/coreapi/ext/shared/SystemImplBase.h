#include "generated/cpp/SystemBase.h"

namespace rho {

using namespace apiGenerator;

class CSystemImplBase: public CSystemBase
{
public:
    CSystemImplBase(): CSystemBase(){}

    virtual void getPlatform(CMethodResult& oResult);
    virtual void getHasCamera(CMethodResult& oResult);
    virtual void getPhoneNumber(CMethodResult& oResult);
    virtual void getDevicePushId(CMethodResult& oResult);
    virtual void getOsVersion(CMethodResult& oResult);
    virtual void getIsMotorolaDevice(CMethodResult& oResult);
    virtual void getLocalServerPort(CMethodResult& oResult);
    virtual void setLocalServerPort( __int64 value, CMethodResult& oResult);
    virtual void getFreeServerPort(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasTouchscreen(rho::apiGenerator::CMethodResult& oResult);
    virtual void getSecurityTokenNotPassed(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasSqlite(rho::apiGenerator::CMethodResult& oResult);

    virtual void getStartParams(rho::apiGenerator::CMethodResult& oResult);
    virtual void unzipFile( const rho::String& localPathToZip,  const rho::String& password, rho::apiGenerator::CMethodResult& oResult);
    virtual void zipFile( const rho::String& localPathToZip,  const rho::String& localPathToFile,  const rho::String& password, CMethodResult& oResult);
    virtual void zipFiles( const rho::String& localPathToZip,  const rho::String& basePath,  const rho::Vector<rho::String>& filePathsToZip,  const rho::String& password, CMethodResult& oResult);
    virtual void setRegistrySetting( const rho::String& keyPath,  const rho::String& keyValue, CMethodResult& oResult);
    virtual void getRegistrySetting( const rho::String& keyPath, CMethodResult& oResult);

    virtual void replaceCurrentBundle( const rho::String& pathToBundle,  const rho::Hashtable<rho::String, rho::String>& params, rho::apiGenerator::CMethodResult& oResult);
    virtual void deleteFolder( const rho::String& pathToFolder, rho::apiGenerator::CMethodResult& oResult);
    virtual void setDoNotBackupAttribute( const rho::String& pathToFile, rho::apiGenerator::CMethodResult& oResult);
    virtual void isBlobAttr( const rho::String& partition,  int64 sourceID,  const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult);
    virtual void updateBlobAttribs( const rho::String& partition,  int64 sourceID, rho::apiGenerator::CMethodResult& oResult);
    virtual void get_locale(rho::apiGenerator::CMethodResult& oResult);
    virtual void setPushNotification( const rho::String& url,  const rho::String& url_params,  const rho::String& push_client, rho::apiGenerator::CMethodResult& oResult);

    virtual void getHasNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasWifiNetwork(rho::apiGenerator::CMethodResult& oResult);
    virtual void getHasCellNetwork(rho::apiGenerator::CMethodResult& oResult);

    virtual void setScreenRotationNotification( const rho::String& url,  const rho::String& url_params, rho::apiGenerator::CMethodResult& oResult);
    virtual void exit(rho::apiGenerator::CMethodResult& oResult);
    virtual void set_sleeping( bool enable, rho::apiGenerator::CMethodResult& oResult);
    virtual void startTimer( int64 interval,  const rho::String& url,  const rho::String& url_params, rho::apiGenerator::CMethodResult& oResult);
    virtual void stopTimer( const rho::String& url, rho::apiGenerator::CMethodResult& oResult);

    virtual void setNetworkStatusNotify( const rho::String& url,  int64 poll_interval, rho::apiGenerator::CMethodResult& oResult);
    virtual void clearNetworkStatusNotify(rho::apiGenerator::CMethodResult& oResult);
    virtual void set_http_proxy_url( const rho::String& proxyURI, rho::apiGenerator::CMethodResult& oResult);
    virtual void unset_http_proxy(rho::apiGenerator::CMethodResult& oResult);

};

}
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

    virtual void getStartParams(rho::apiGenerator::CMethodResult& oResult);
    virtual void unzipFile( const rho::StringW& localPathToZip,  const rho::StringW& password, rho::apiGenerator::CMethodResult& oResult);
    virtual void zipFile( const rho::StringW& localPathToZip,  const rho::StringW& localPathToFile,  const rho::StringW& password, CMethodResult& oResult);
    virtual void zipFiles( const rho::StringW& localPathToZip,  const rho::StringW& basePath,  const rho::Vector<rho::StringW>& filePathsToZip,  const rho::StringW& password, CMethodResult& oResult);
    virtual void replaceCurrentBundle( const rho::StringW& pathToBundle, rho::apiGenerator::CMethodResult& oResult);
    virtual void deleteFolder( const rho::StringW& pathToFolder, rho::apiGenerator::CMethodResult& oResult);
    virtual void setDoNotBackupAttribute( const rho::StringW& pathToFile, rho::apiGenerator::CMethodResult& oResult);
    virtual void isBlobAttr( const rho::StringW& partition,  int64 sourceID,  const rho::StringW& attrName, rho::apiGenerator::CMethodResult& oResult);
    virtual void updateBlobAttribs( const rho::StringW& partition,  int64 sourceID, rho::apiGenerator::CMethodResult& oResult);

};

}
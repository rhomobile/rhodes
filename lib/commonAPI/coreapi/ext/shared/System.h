#include "generated/cpp/SystemBase.h"
#include "SystemImplBase.h"

namespace rho {

using namespace rho::apiGenerator;

struct System
{
    static ISystemSingleton* getSystemObj(){ return CSystemFactoryBase::getSystemSingletonS(); }

    static rho::String getPlatform(){ rho::apiGenerator::CMethodResult oResult(true); getSystemObj()->getPlatform(oResult); return oResult.getString(); }
    static rho::String getDeviceName(){ rho::apiGenerator::CMethodResult oResult(true); getSystemObj()->getDeviceName(oResult); return oResult.getString(); }
    static rho::String getOsVersion(){ rho::apiGenerator::CMethodResult oResult(true); getSystemObj()->getOsVersion(oResult); return oResult.getString(); }
    static rho::String getPhoneId(){ rho::apiGenerator::CMethodResult oResult(true); getSystemObj()->getPhoneId(oResult); return oResult.getString(); }
    static rho::String getWebviewFramework(){ rho::apiGenerator::CMethodResult oResult(true); getSystemObj()->getWebviewFramework(oResult); return oResult.getString(); }
    static bool getIsRhoSimulator(){ rho::apiGenerator::CMethodResult oResult(true); getSystemObj()->getIsRhoSimulator(oResult); return oResult.getBool(); }
};

}

#include "generated/cpp/NetworkBase.h"

namespace rho {

using namespace rho::apiGenerator;

struct Network
{
    static INetworkSingleton* getNetworkObj(){ return CNetworkFactoryBase::getNetworkSingletonS(); }

    static bool hasNetwork(){ rho::apiGenerator::CMethodResult oResult(true); getNetworkObj()->hasNetwork(oResult); return oResult.getBool(); }
    static bool hasWifiNetwork(){ rho::apiGenerator::CMethodResult oResult(true);  getNetworkObj()->hasWifiNetwork(oResult); return oResult.getBool(); }
    static bool hasCellNetwork(){ rho::apiGenerator::CMethodResult oResult(true);  getNetworkObj()->hasCellNetwork(oResult); return oResult.getBool(); }
};

}

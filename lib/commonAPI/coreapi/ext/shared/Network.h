#include "generated/cpp/NetworkAccessBase.h"

namespace rho {

using namespace rho::apiGenerator;

struct Network
{
    static INetworkAccessSingleton* getNetworkAccessObj(){ return CNetworkAccessFactoryBase::getNetworkAccessSingletonS(); }

    static bool hasNetwork(){ rho::apiGenerator::CMethodResult oResult(true); getNetworkAccessObj()->hasNetwork(oResult); return oResult.getBool(); }
    static bool hasWifiNetwork(){ rho::apiGenerator::CMethodResult oResult(true);  getNetworkAccessObj()->hasWifiNetwork(oResult); return oResult.getBool(); }
    static bool hasCellNetwork(){ rho::apiGenerator::CMethodResult oResult(true);  getNetworkAccessObj()->hasCellNetwork(oResult); return oResult.getBool(); }
};

}

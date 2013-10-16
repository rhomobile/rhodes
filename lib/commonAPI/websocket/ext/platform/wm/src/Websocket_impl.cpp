#include "../../../shared/generated/cpp/WebsocketBase.h"

namespace rho {

using namespace apiGenerator;

class CWebsocketImpl: public CWebsocketBase
{
public:
    CWebsocketImpl(const rho::String& strID): CWebsocketBase()
    {
    }

    virtual void getPlatformName(rho::apiGenerator::CMethodResult& oResult) {
         oResult.set("WM");
    }

    virtual void calcSumm( int a,  int b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }
    
    virtual void joinStrings( const rho::String& a,  const rho::String& b, rho::apiGenerator::CMethodResult& oResult) {
         oResult.set(a+b);
    }

};

class CWebsocketSingleton: public CWebsocketSingletonBase
{
    ~CWebsocketSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CWebsocketFactory: public CWebsocketFactoryBase
{
    ~CWebsocketFactory(){}
    virtual IWebsocketSingleton* createModuleSingleton();
    virtual IWebsocket* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Websocket_extension()
{
    CWebsocketFactory::setInstance( new CWebsocketFactory() );
    Init_Websocket_API();
}

IWebsocket* CWebsocketFactory::createModuleByID(const rho::String& strID)
{
    return new CWebsocketImpl(strID);
}

IWebsocketSingleton* CWebsocketFactory::createModuleSingleton()
{
    return new CWebsocketSingleton();
}

void CWebsocketSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CWebsocketSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
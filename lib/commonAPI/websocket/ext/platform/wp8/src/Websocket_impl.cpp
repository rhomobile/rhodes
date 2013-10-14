#include "../../../shared/generated/cpp/WebsocketBase.h"

namespace rho {

using namespace apiGenerator;

class CWebsocketImpl: public CWebsocketBase
{
public:
    CWebsocketImpl(const rho::String& strID): CWebsocketBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

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
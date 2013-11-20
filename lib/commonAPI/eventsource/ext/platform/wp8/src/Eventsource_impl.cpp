#include "../../../shared/generated/cpp/EventsourceBase.h"

namespace rho {

using namespace apiGenerator;

class CEventsourceImpl: public CEventsourceBase
{
public:
    CEventsourceImpl(const rho::String& strID): CEventsourceBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CEventsourceSingleton: public CEventsourceSingletonBase
{
    ~CEventsourceSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CEventsourceFactory: public CEventsourceFactoryBase
{
    ~CEventsourceFactory(){}
    virtual IEventsourceSingleton* createModuleSingleton();
    virtual IEventsource* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Eventsource_extension()
{
    CEventsourceFactory::setInstance( new CEventsourceFactory() );
    Init_Eventsource_API();
}

IEventsource* CEventsourceFactory::createModuleByID(const rho::String& strID)
{
    return new CEventsourceImpl(strID);
}

IEventsourceSingleton* CEventsourceFactory::createModuleSingleton()
{
    return new CEventsourceSingleton();
}

void CEventsourceSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CEventsourceSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
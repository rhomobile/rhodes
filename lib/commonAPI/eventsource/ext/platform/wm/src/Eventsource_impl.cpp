#include "../../../shared/generated/cpp/EventsourceBase.h"

namespace rho {

using namespace apiGenerator;

class CEventsourceImpl: public CEventsourceBase
{
public:
    CEventsourceImpl(const rho::String& strID): CEventsourceBase()
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
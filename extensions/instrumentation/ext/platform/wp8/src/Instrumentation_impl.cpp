#include "../../../shared/generated/cpp/InstrumentationBase.h"

namespace rho {

using namespace apiGenerator;

class CInstrumentationImpl: public CInstrumentationBase
{
public:
    CInstrumentationImpl(const rho::String& strID): CInstrumentationBase()
    {
        m_hashProps.put( "display", "LCD");
        m_hashProps.put( "sound", "Dolby");
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CInstrumentationSingleton: public CInstrumentationSingletonBase
{
    ~CInstrumentationSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CInstrumentationFactory: public CInstrumentationFactoryBase
{
    ~CInstrumentationFactory(){}
    virtual IInstrumentationSingleton* createModuleSingleton();
    virtual IInstrumentation* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Instrumentation_extension()
{
    CInstrumentationFactory::setInstance( new CInstrumentationFactory() );
    Init_Instrumentation_API();
}

IInstrumentation* CInstrumentationFactory::createModuleByID(const rho::String& strID)
{
    return new CInstrumentationImpl(strID);
}

IInstrumentationSingleton* CInstrumentationFactory::createModuleSingleton()
{
    return new CInstrumentationSingleton();
}

void CInstrumentationSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CInstrumentationSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
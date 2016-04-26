#include "../../../shared/generated/cpp/PrintingZebraBase.h"

namespace rho {

using namespace apiGenerator;

class CPrintingZebraImpl: public CPrintingZebraBase
{
public:
    CPrintingZebraImpl(const rho::String& strID): CPrintingZebraBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CPrintingZebraSingleton: public CPrintingZebraSingletonBase
{
    ~CPrintingZebraSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CPrintingZebraFactory: public CPrintingZebraFactoryBase
{
    ~CPrintingZebraFactory(){}
    virtual IPrintingZebraSingleton* createModuleSingleton();
    virtual IPrintingZebra* createModuleByID(const rho::String& strID);
};

extern "C" void Init_PrintingZebra_extension()
{
    CPrintingZebraFactory::setInstance( new CPrintingZebraFactory() );
    Init_PrintingZebra_API();
}

IPrintingZebra* CPrintingZebraFactory::createModuleByID(const rho::String& strID)
{
    return new CPrintingZebraImpl(strID);
}

IPrintingZebraSingleton* CPrintingZebraFactory::createModuleSingleton()
{
    return new CPrintingZebraSingleton();
}

void CPrintingZebraSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CPrintingZebraSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
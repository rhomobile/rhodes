#include "../../../shared/generated/cpp/PrintingBase.h"

namespace rho {

using namespace apiGenerator;

class CPrintingImpl: public CPrintingBase
{
public:
    CPrintingImpl(const rho::String& strID): CPrintingBase()
    {
    }

    virtual void enable( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}
    virtual void start(CMethodResult& oResult){}
    virtual void stop(CMethodResult& oResult){}
    virtual void disable(CMethodResult& oResult){}
    virtual void take( const rho::Hashtable<rho::String, rho::String>& propertyMap, CMethodResult& oResult){}

};

class CPrintingSingleton: public CPrintingSingletonBase
{
    ~CPrintingSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CPrintingFactory: public CPrintingFactoryBase
{
    ~CPrintingFactory(){}
    virtual IPrintingSingleton* createModuleSingleton();
    virtual IPrinting* createModuleByID(const rho::String& strID);
};

extern "C" void Init_Printing_extension()
{
    CPrintingFactory::setInstance( new CPrintingFactory() );
    Init_Printing_API();
}

IPrinting* CPrintingFactory::createModuleByID(const rho::String& strID)
{
    return new CPrintingImpl(strID);
}

IPrintingSingleton* CPrintingFactory::createModuleSingleton()
{
    return new CPrintingSingleton();
}

void CPrintingSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CPrintingSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
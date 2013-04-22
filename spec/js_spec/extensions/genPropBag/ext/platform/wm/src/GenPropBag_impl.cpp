#include "../../../shared/generated/cpp/genPropBagBase.h"

namespace rho {

using namespace apiGenerator;

class CGenPropBagImpl: public CGenPropBagBase
{
public:
    CGenPropBagImpl(const rho::String& strID): CGenPropBagBase()
    {
    }


};

class CGenPropBagSingleton: public CGenPropBagSingletonBase
{
    ~CGenPropBagSingleton(){}
    virtual rho::String getInitialDefaultID();
    void enumerate(CMethodResult& oResult);

    virtual void testMethod1(rho::apiGenerator::CMethodResult& oResult)
    {
        oResult.set(false);
    }

};

class CGenPropBagFactory: public CGenPropBagFactoryBase
{
    ~CGenPropBagFactory(){}
    virtual IGenPropBagSingleton* createModuleSingleton();
    virtual IGenPropBag* createModuleByID(const rho::String& strID);
};

extern "C" void Init_GenPropBag_extension()
{
    CGenPropBagFactory::setInstance( new CGenPropBagFactory() );
    Init_GenPropBag_API();
}

IGenPropBag* CGenPropBagFactory::createModuleByID(const rho::String& strID)
{
    return new CGenPropBagImpl(strID);
}

IGenPropBagSingleton* CGenPropBagFactory::createModuleSingleton()
{
    return new CGenPropBagSingleton();
}

void CGenPropBagSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("ID1");
    arIDs.addElement("ID2");

    oResult.set(arIDs);
}

rho::String CGenPropBagSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
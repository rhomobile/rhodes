#include "../../../shared/generated/cpp/PrintingEpsonBase.h"

namespace rho {

using namespace apiGenerator;

class CPrintingEpsonImpl: public CPrintingEpsonBase
{
public:
    CPrintingEpsonImpl(const rho::String& strID): CPrintingEpsonBase()
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

class CPrintingEpsonSingleton: public CPrintingEpsonSingletonBase
{
    ~CPrintingEpsonSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CPrintingEpsonFactory: public CPrintingEpsonFactoryBase
{
    ~CPrintingEpsonFactory(){}
    virtual IPrintingEpsonSingleton* createModuleSingleton();
    virtual IPrintingEpson* createModuleByID(const rho::String& strID);
};

extern "C" void Init_PrintingEpson_extension()
{
    CPrintingEpsonFactory::setInstance( new CPrintingEpsonFactory() );
    Init_PrintingEpson_API();
}

IPrintingEpson* CPrintingEpsonFactory::createModuleByID(const rho::String& strID)
{
    return new CPrintingEpsonImpl(strID);
}

IPrintingEpsonSingleton* CPrintingEpsonFactory::createModuleSingleton()
{
    return new CPrintingEpsonSingleton();
}

void CPrintingEpsonSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CPrintingEpsonSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
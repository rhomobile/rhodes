#include "../../../shared/generated/cpp/JavascriptVMBase.h"

namespace rho {

using namespace apiGenerator;

class CJavascriptVMImpl: public CJavascriptVMBase
{
public:
    CJavascriptVMImpl(const rho::String& strID): CJavascriptVMBase()
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

class CJavascriptVMSingleton: public CJavascriptVMSingletonBase
{
    ~CJavascriptVMSingleton(){}
    virtual rho::String getInitialDefaultID();
    virtual void enumerate(CMethodResult& oResult);
};

class CJavascriptVMFactory: public CJavascriptVMFactoryBase
{
    ~CJavascriptVMFactory(){}
    virtual IJavascriptVMSingleton* createModuleSingleton();
    virtual IJavascriptVM* createModuleByID(const rho::String& strID);
};

extern "C" void Init_JavascriptVM_extension()
{
    CJavascriptVMFactory::setInstance( new CJavascriptVMFactory() );
    Init_JavascriptVM_API();
}

IJavascriptVM* CJavascriptVMFactory::createModuleByID(const rho::String& strID)
{
    return new CJavascriptVMImpl(strID);
}

IJavascriptVMSingleton* CJavascriptVMFactory::createModuleSingleton()
{
    return new CJavascriptVMSingleton();
}

void CJavascriptVMSingleton::enumerate(CMethodResult& oResult)
{
    rho::Vector<rho::String> arIDs;
    arIDs.addElement("SC1");
    arIDs.addElement("SC2");

    oResult.set(arIDs);
}

rho::String CJavascriptVMSingleton::getInitialDefaultID()
{
    CMethodResult oRes;
    enumerate(oRes);

    rho::Vector<rho::String>& arIDs = oRes.getStringArray();
        
    return arIDs[0];
}

}
#include "IJavascriptVM.h"
#include "common/StringConverter.h"


namespace rho {

using namespace rho::apiGenerator;

class CJavascriptVMFactoryBase : public CModuleFactoryBase<IJavascriptVM, IJavascriptVMSingleton, IJavascriptVMFactory>
{
protected:
    static rho::common::CAutoPtr<CJavascriptVMFactoryBase> m_pInstance;
    Hashtable<rho::String,IJavascriptVM*> m_hashModules;

public:

    static void setInstance(CJavascriptVMFactoryBase* pInstance){ m_pInstance = pInstance; }
    static CJavascriptVMFactoryBase* getInstance(){ return m_pInstance; }

    static IJavascriptVMSingleton* getJavascriptVMSingletonS(){ return getInstance()->getModuleSingleton(); }

    virtual IJavascriptVM* getModuleByID(const rho::String& strID)
    {
        if ( !m_hashModules.containsKey(strID) )
        {
            IJavascriptVM* pObj = createModuleByID(strID);
            m_hashModules.put(strID, pObj );

            return pObj;
        }

        return m_hashModules[strID];
    }

    virtual IJavascriptVM* createModuleByID(const rho::String& strID){ return (IJavascriptVM*)0; };

};

class CJavascriptVMSingletonBase : public CModuleSingletonBase< IJavascriptVMSingleton >
{
protected:




public:




};

class CJavascriptVMBase: public IJavascriptVM
{
protected:

public:

    CJavascriptVMBase();


 

};

extern "C" void Init_JavascriptVM_API();


}

#pragma once

#include "common/RhoStd.h"
#include "../../common/MethodResult.h"
#include "../../common/BaseClasses.h"

///////////////////////////////////////////////////////////
struct IBarcode1
{
    virtual ~IBarcode1(){}
    virtual void getProps(CMethodResult& oResult) = 0;
    virtual void getPropsWithString(const rho::String& strName, CMethodResult& oResult) = 0;
    virtual void getPropsWithArray(const rho::Vector<rho::String>& arNames, CMethodResult& oResult) = 0;
};

struct IBarcode1Singleton
{
    virtual ~IBarcode1Singleton(){}

    virtual void enumerate(CMethodResult& oResult) = 0;

    virtual rho::String getDefaultID() = 0;
    virtual rho::String getInitialDefaultID() = 0;
    virtual void setDefaultID(const rho::String& strID) = 0;

    virtual void addCommandToQueue(rho::common::IRhoRunnable* pFunctor) = 0;
};

struct IBarcode1Factory
{
    virtual ~IBarcode1Factory(){}

    virtual IBarcode1Singleton* getModuleSingleton() = 0;
    virtual IBarcode1* getModuleByID(const rho::String& strID) = 0;
};

class CBarcode1FactoryBase : public CModuleFactoryBase<IBarcode1, IBarcode1Singleton, IBarcode1Factory>
{
protected:
    static rho::common::CAutoPtr<CBarcode1FactoryBase> m_pInstance;

public:

    static void setInstance(CBarcode1FactoryBase* pInstance){ m_pInstance = pInstance; }
    static CBarcode1FactoryBase* getInstance(){ return m_pInstance; }

    static IBarcode1Singleton* getBarcode1SingletonS(){ return getInstance()->getModuleSingleton(); }
};

extern "C" void Init_RubyAPI_Barcode1();
extern "C" void Init_JSAPI_Barcode1();

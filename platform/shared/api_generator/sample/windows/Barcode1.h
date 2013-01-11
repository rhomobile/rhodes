#pragma once

#include "common/RhoStd.h"
#include "../../common/MethodResult.h"
#include "../../common/BaseClasses.h"

///////////////////////////////////////////////////////////
struct IBarcode1
{
    virtual ~IBarcode1(){}
    virtual void getProps(CMethodResult& oResult) = 0;
    virtual void getProps(const rho::String& strName, CMethodResult& oResult) = 0;
    virtual void getProps(const rho::Vector<rho::String>& arNames, CMethodResult& oResult) = 0;
};

struct IBarcode1Singleton
{
    virtual ~IBarcode1Singleton(){}
    virtual IBarcode1* create(const rho::String& strID) = 0;
    virtual rho::String getDefaultID() = 0;
    virtual void enumerate(CMethodResult& oResult) = 0;
};


class CBarcode1SingletonBase : public CModuleSingletonBase<IBarcode1, IBarcode1Singleton>
{
protected:
    static rho::common::CAutoPtr<CBarcode1SingletonBase> m_pInstance;

public:
    static void setInstance(CBarcode1SingletonBase* pInstance){ m_pInstance = pInstance; }
    static CBarcode1SingletonBase* getInstance(){ return m_pInstance; }

};

extern "C" void Init_RubyAPI_Barcode1();
extern "C" void Init_JSAPI_Barcode1();

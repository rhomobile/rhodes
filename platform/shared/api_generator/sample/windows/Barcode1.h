#pragma once

#include "common/RhoStd.h"
#include "../../common/MethodResult.h"
#include "../../common/GeneratorQueue.h"

///////////////////////////////////////////////////////////
struct IBarcode1
{
    virtual ~IBarcode1(){}
    virtual void getProps(CMethodResult& oResult) = 0;
    virtual void getProps(const rho::String& strName, CMethodResult& oResult) = 0;
    virtual void getProps(const rho::Vector<rho::String>& arNames, CMethodResult& oResult) = 0;
};

class CBarcode1ImplBase: public IBarcode1
{
protected:

    rho::String m_strID;
    rho::Hashtable<rho::String, rho::String> m_hashProps;

public:
    CBarcode1ImplBase(const rho::String& strID)
    {
        m_strID = strID;
    }

    virtual void getProps(CMethodResult& oResult)
    { 
        oResult.set(m_hashProps);
    }

    virtual void getProps(const rho::String& strName, CMethodResult& oResult)
    {
        oResult.set(m_hashProps[strName]);
    }
    virtual void getProps(const rho::Vector<rho::String>& arNames, CMethodResult& oResult)
    {
        oResult.set(m_hashProps);
    }

};

struct IBarcode1Singleton
{
    virtual ~IBarcode1Singleton(){}
    virtual IBarcode1* create(const rho::String& strID) = 0;
    virtual rho::String getDefaultID() = 0;
    virtual void enumerate(CMethodResult& oResult) = 0;
};

class CBarcode1SingletonBase;
CBarcode1SingletonBase* barcode1_impl_createSingleton();

class CBarcode1SingletonBase : public IBarcode1Singleton
{
protected:
    rho::String m_strDefaultID;
    rho::Hashtable<rho::String,IBarcode1*> m_hashBarcodes;
    rho::common::CAutoPtr<rho::common::CThreadQueue> m_pCommandQueue;
    static rho::common::CAutoPtr<CBarcode1SingletonBase> m_pInstance;

public:
    static CBarcode1SingletonBase* getInstance()
    {
        if (!m_pInstance)
            m_pInstance = barcode1_impl_createSingleton();

        return m_pInstance;
    }

    rho::Hashtable<rho::String,IBarcode1*>& getBarcodes(){ return m_hashBarcodes; }

    void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }

    void setCommandQueue( rho::common::CThreadQueue* pQueue){ m_pCommandQueue = pQueue; }
    void addCommandToQueue(rho::common::IRhoRunnable* pFunctor)
    {
        if ( !m_pCommandQueue )
        {
            m_pCommandQueue = new CGeneratorQueue();
            m_pCommandQueue->start(rho::common::CThreadQueue::epLow);
        }

        m_pCommandQueue->addQueueCommand( new CGeneratorQueue::CGeneratorQueueCommand(pFunctor) );
    }

    rho::String getDefaultIDEx()
    { 
        if ( m_strDefaultID.length() == 0 )
            setDefaultID(getDefaultID());

        if ( !getBarcodes().containsKey(m_strDefaultID) )
        {
            IBarcode1* pObj = create(m_strDefaultID);
            getBarcodes().put(m_strDefaultID, pObj );
        }
        return m_strDefaultID; 
    }
};


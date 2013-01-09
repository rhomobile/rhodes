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

class CBarcode1FactoryBase
{
protected:
    static rho::String m_strDefaultID;
    static rho::Hashtable<rho::String,IBarcode1*> m_hashBarcodes;
    static rho::common::CAutoPtr<rho::common::CThreadQueue> m_pCommandQueue;

public:
    static rho::Hashtable<rho::String,IBarcode1*>& getBarcodes(){ return m_hashBarcodes; }

    static void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }

    static void setCommandQueue( rho::common::CThreadQueue* pQueue){ m_pCommandQueue = pQueue; }
    static void addCommandToQueue(rho::common::IRhoRunnable* pFunctor)
    {
        if ( !m_pCommandQueue )
        {
            m_pCommandQueue = new CGeneratorQueue();
            m_pCommandQueue->start(rho::common::CThreadQueue::epLow);
        }

        m_pCommandQueue->addQueueCommand( new CGeneratorQueue::CGeneratorQueueCommand(pFunctor) );
    }
};

class CBarcode1 : public CBarcode1FactoryBase
{
public:
    static IBarcode1* create(const rho::String& strID);
    static rho::String getDefaultID();
    static void enumerate(CMethodResult& oResult);


    static rho::String getDefaultIDEx()
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
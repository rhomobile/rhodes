#pragma once

#include "common/RhoStd.h"
#include "common/ThreadQueue.h"

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H

class CMethodResult
{
    rho::String m_strRubyCallback, m_strCallbackParam, m_strStringParam;
    rho::String m_strJSCallback;
    rho::Hashtable<rho::String, rho::String> m_hashStrRes;
    rho::String m_strRes;
    rho::Vector<rho::String> m_arStrRes;
    rho::String m_strError;
    boolean m_bCallInUIThread;

    enum ETypes{ eNone = 0, eString, eStringArray, eStringHash, eError, eArgError};
    ETypes m_ResType;
public:

    void setRubyCallback(const rho::String& strCallback){ m_strRubyCallback = strCallback; }
    void setJSCallback(const rho::String& strCallback){ m_strJSCallback = strCallback; }
    void setCallInUIThread(boolean bUIThread){ m_bCallInUIThread = bUIThread; }
    void setCallbackParam(const rho::String& strCallbackParam){ m_strCallbackParam = strCallbackParam; }
    void setStringParam(const rho::String& strParam){m_strStringParam = strParam;}
    void convertStringParamToHash()
    {
        if ( m_ResType == eString )
        {
            rho::Hashtable<rho::String, rho::String> resHash;
            resHash.put( m_strStringParam, m_strRes);
            set(resHash);
        }
    }

    void set(const rho::Hashtable<rho::String, rho::String>& res){ m_hashStrRes = res; m_ResType = eStringHash; }
    void set(const rho::String& res){ m_strRes = res;  m_ResType = eString; }
    void set(const rho::Vector<rho::String>& res){ m_arStrRes = res;  m_ResType = eStringArray; }
    void setError(const rho::String& res){ m_strError = res; m_ResType = eError; }
    void setArgError(const char *fmt, ...)
    {
        //TODO: support sprintf
        m_strError = fmt;
        m_ResType = eArgError;
    }

    rho::Vector<rho::String>& getStringArray(){ return m_arStrRes; }

    VALUE toRuby();
    rho::String toJSON();

    void callCallback();
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE1, typename PARAMTYPE2 >
class CObjCallbackFunctor2 : public rho::common::CInstanceClassFunctor2<OBJTYPE, FUNCTYPE, PARAMTYPE1, PARAMTYPE2>
{
public:
    CObjCallbackFunctor2( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE1 param1, PARAMTYPE2 param2 ) : CInstanceClassFunctor2( obj, pFunc, param1, param2 ){}

    virtual void runObject()
    { 
        rho::common::CInstanceClassFunctor2<OBJTYPE, FUNCTYPE, PARAMTYPE1, PARAMTYPE2>::runObject();

        m_param2.callCallback();
    }
};

template <typename OBJTYPE, typename FUNCTYPE, typename PARAMTYPE >
class CObjCallbackFunctor1 : public rho::common::CInstanceClassFunctor1<OBJTYPE, FUNCTYPE, PARAMTYPE>
{
public:
    CObjCallbackFunctor1( OBJTYPE obj, FUNCTYPE pFunc, PARAMTYPE param ) : CInstanceClassFunctor1( obj, pFunc, param ){}

    virtual void runObject()
    { 
        rho::common::CInstanceClassFunctor1<OBJTYPE, FUNCTYPE, PARAMTYPE>::runObject();

        m_param.callCallback();
    }
};

class CGeneratorQueue : public rho::common::CThreadQueue
{
public:

    class CGeneratorQueueCommand: public rho::common::CThreadQueue::IQueueCommand
    {
        rho::common::IRhoRunnable* m_pFunctor;
    public:
        CGeneratorQueueCommand(rho::common::IRhoRunnable* pFunctor) : m_pFunctor(pFunctor){}
        virtual ~CGeneratorQueueCommand(){};
        virtual bool equals(const IQueueCommand& cmd)
        {
            return false;
        }
        virtual rho::String toString()
        {
            return "";
        }
        virtual void cancel(){}

        virtual void execute()
        {
            m_pFunctor->runObject();
        }
    };

    CGeneratorQueue() : CThreadQueue()
    {
        //TODO: use log category from CBarcode1
        //CThreadQueue::setLogCategory(getLogCategory());

        setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
    }
};

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

class CBarcode1
{
    static rho::String m_strDefaultID;
    static rho::Hashtable<rho::String,IBarcode1*> m_hashBarcodes;
    static rho::common::CAutoPtr<CGeneratorQueue> m_pCommandQueue;

public:
    static IBarcode1* create(const rho::String& strID);
    static rho::Hashtable<rho::String,IBarcode1*>& getBarcodes(){ return m_hashBarcodes; }

    static rho::String getDefaultID()
    { 
        if ( m_strDefaultID.length() == 0 )
            CBarcode1::initDefaultID();

        if ( !getBarcodes().containsKey(m_strDefaultID) )
        {
            IBarcode1* pObj = create(m_strDefaultID);
            getBarcodes().put(m_strDefaultID, pObj );
        }
        return m_strDefaultID; 
    }

    static void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }
    static void CBarcode1::initDefaultID();

    static void enumerate(CMethodResult& oResult);

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
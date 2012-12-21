#pragma once

#include "common/RhoStd.h"

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H

class CMethodResult
{
    rho::String m_strCallback, m_strCallbackParam;
    rho::Hashtable<rho::String, rho::String> m_hashStrRes;
    rho::String m_strRes;
    rho::Vector<rho::String> m_arStrRes;
    rho::String m_strError;

    enum ETypes{ eNone = 0, eString, eStringArray, eStringHash, eError, eArgError};
    ETypes m_ResType;
public:

    void setCallback(const rho::String& strCallback){ m_strCallback = strCallback; }
    void setCallbackParam(const rho::String& strCallbackParam){ m_strCallbackParam = strCallbackParam; }
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
    rho::String toJSON(){ return "{}";}

    void callCallback();
};

struct IBarcode1
{
    virtual ~IBarcode1(){}
    virtual void getProps(CMethodResult& oResult) = 0;
    virtual void getProps(const rho::String& strName, CMethodResult& oResult) = 0;
    virtual void getProps(const rho::Vector<rho::String>& arNames, CMethodResult& oResult) = 0;
};

class CBarcode1ImplBase: public IBarcode1
{
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

};
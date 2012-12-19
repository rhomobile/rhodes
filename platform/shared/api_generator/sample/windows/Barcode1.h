#pragma once

#include "common/RhoStd.h"

#ifndef RUBY_RUBY_H
typedef unsigned long VALUE;
#endif //!RUBY_RUBY_H

class CMethodResult
{
    rho::String m_strCallback;
    rho::Hashtable<rho::String, rho::String> m_hashStrRes;
    rho::String m_strRes;

    enum ETypes{ eNone = 0, eString, eStringArray, eStringHash};
    ETypes m_ResType;
public:

    void setCallback(const rho::String& strCallback){ m_strCallback = strCallback; }
    void set(const rho::Hashtable<rho::String, rho::String>& res){ m_hashStrRes = res; m_ResType = eStringHash; }
    void set(const rho::String& res){ m_strRes = res;  m_ResType = eString; }

    VALUE toRuby();
    rho::String toJSON(){ return "{}";}
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
public:
    static IBarcode1* create(const rho::String& strID);
    static rho::String getDefaultID(){ return m_strDefaultID; }
    static void setDefaultID(const rho::String& strDefaultID){ m_strDefaultID = strDefaultID; }
    static void CBarcode1::initDefaultID();

    static rho::Vector<rho::String> enumerate();

};
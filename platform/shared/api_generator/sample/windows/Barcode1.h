#pragma once

#include "common/RhoStd.h"

class CCallback
{
    rho::String m_strCallback;
public:
    CCallback(const rho::String& strCallback): m_strCallback(strCallback){}
};

struct IBarcode1
{
    virtual ~IBarcode1(){}
    virtual rho::Hashtable<rho::String, rho::String> getProps(CCallback* pCallback = 0) = 0;
    virtual rho::String getProps(const rho::String& strName, CCallback* pCallback = 0) = 0;
    virtual rho::Hashtable<rho::String, rho::String> getProps(const rho::Vector<rho::String>& arNames, CCallback* pCallback = 0) = 0;
};

class CBarcode1ImplBase: public IBarcode1
{
    rho::String m_strID;
    rho::Hashtable<rho::String, rho::String> m_hashProps;
    CCallback* m_pCallback;

public:
    CBarcode1ImplBase(const rho::String& strID) : m_pCallback(0)
    {
        m_strID = strID;
    }

    virtual rho::Hashtable<rho::String, rho::String> getProps(CCallback* pCallback = 0){return m_hashProps;}
    virtual rho::String getProps(const rho::String& strName, CCallback* pCallback = 0)
    {
        return m_hashProps[strName];
    }
    virtual rho::Hashtable<rho::String, rho::String> getProps(const rho::Vector<rho::String>& arNames, CCallback* pCallback = 0)
    {
        return m_hashProps;
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
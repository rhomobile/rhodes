#pragma once

#include "common/RhoStd.h"
/*
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
};*/

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
            m_hashStrRes = resHash; m_ResType = eStringHash;
        }
    }

    void set(const rho::Hashtable<rho::String, rho::String>& res){ m_hashStrRes = res; m_ResType = eStringHash; callCallback(); }
    void set(const rho::String& res){ m_strRes = res;  m_ResType = eString; callCallback(); }
    void set(const rho::Vector<rho::String>& res){ m_arStrRes = res;  m_ResType = eStringArray; callCallback(); }
    void setError(const rho::String& res){ m_strError = res; m_ResType = eError; callCallback(); }
    void setArgError(const char *fmt, ...)
    {
        //TODO: support sprintf
        m_strError = fmt;
        m_ResType = eArgError;
        callCallback();
    }

    rho::Vector<rho::String>& getStringArray(){ return m_arStrRes; }

    unsigned long toRuby();
    rho::String toJSON();

    void callCallback();
};

#pragma once

#include "common/RhoStd.h"
#include "common/AutoPointer.h"
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

namespace rho
{
namespace apiGenerator
{

class CMethodResult
{
    rho::String m_strRubyCallback, m_strCallbackParam, m_strStringParam;
    rho::String m_strJSCallback;
    rho::Hashtable<rho::String, rho::String> m_hashStrRes;
    rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> > m_hashStrL2Res;
    rho::String m_strRes;
    rho::StringW m_strResW;
    rho::Vector<rho::String> m_arStrRes;
    int64 m_nRes;
    bool m_bRes;
    double m_dRes;

    rho::String m_strError;
    rho::boolean m_bCallInUIThread;
    
    enum ETypes{ eNone = 0, eString, eStringW, eStringArray, eStringHash, eBool, eInt, eDouble, eError, eArgError};
    ETypes m_ResType;

    unsigned long m_oRubyObjectClass;
    rho::String   m_strRubyObjectClassPath;
    bool m_bCollectionMode;

    struct CMethodRubyValue
    {
        unsigned long m_value;
        CMethodRubyValue(unsigned long val);
        ~CMethodRubyValue();

        unsigned long getValue(){return m_value;}
        operator unsigned long() {return m_value;}
    };

    rho::common::CAutoPtr<CMethodRubyValue> m_pRubyCallbackProc;
public:

    CMethodResult(bool bCollectionMode=false): m_ResType(eNone), m_oRubyObjectClass(0), m_bCallInUIThread(false), m_bCollectionMode(bCollectionMode){}

    void setRubyCallback(const rho::String& strCallback){ m_strRubyCallback = strCallback; }
    void setRubyCallbackProc(unsigned long oRubyCallbackProc);
    void setJSCallback(const rho::String& strCallback){ m_strJSCallback = strCallback; }
    void setCallInUIThread(rho::boolean bUIThread){ m_bCallInUIThread = bUIThread; }
    void setCallbackParam(const rho::String& strCallbackParam){ m_strCallbackParam = strCallbackParam; }
    void setStringParam(const rho::String& strParam){m_strStringParam = strParam;}
    void setRubyObjectClass(unsigned long val){ m_oRubyObjectClass = val; }
    void setRubyObjectClassPath(const rho::String& strPath){ m_strRubyObjectClassPath = strPath; }
    void convertStringParamToHash();

    void set(const rho::Hashtable<rho::String, rho::String>& res){ m_hashStrRes = res; m_ResType = eStringHash; callCallback(); }
#ifndef OS_ANDROID
    void set(const rho::StringW& res){ m_strResW = res;  m_ResType = eStringW; callCallback(); }
    void set(rho::StringW::const_pointer res){ m_strResW = res;  m_ResType = eStringW; callCallback(); }
#endif //OS_ANDROID

    void set(const rho::String& res){ m_strRes = res;  m_ResType = eString; callCallback(); }
    void set(rho::String::const_pointer res){ m_strRes = res;  m_ResType = eString; callCallback(); }

    void set(const rho::Vector<rho::String>& res){ m_arStrRes = res;  m_ResType = eStringArray; callCallback(); }
    void set(bool res){ m_bRes = res;  m_ResType = eBool; callCallback(); }
    void set(int64 res){ m_nRes = res;  m_ResType = eInt; callCallback(); }
    void set(int res){ m_nRes = res;  m_ResType = eInt; callCallback(); }
    void set(unsigned long res){ m_nRes = res;  m_ResType = eInt; callCallback(); }
    void set(double res){ m_dRes = res;  m_ResType = eDouble; callCallback(); }

    void setError(const rho::String& res){ m_strError = res; m_ResType = eError; callCallback(); }
    void setArgError(const rho::String& fmt, ...)
    {
        //TODO: support sprintf
        m_strError = fmt;
        m_ResType = eArgError;
        callCallback();
    }

    bool isError(){ return m_ResType == eError || m_ResType == eArgError; }

    rho::Vector<rho::String>& getStringArray(){ return m_arStrRes; }
    rho::Hashtable<rho::String, rho::String>& getStringHash(){ return m_hashStrRes; }
    rho::Hashtable<rho::String, rho::Hashtable<rho::String, rho::String> >& getStringHashL2(){ return m_hashStrL2Res; }
    rho::String& getString(){ return m_strRes; }

    rho::String toString();
    void setCollectionMode(bool bMode){m_bCollectionMode = bMode;}

    unsigned long toRuby();
    rho::String toJSON();

    bool hasCallback();
    void callCallback();
};

}
}

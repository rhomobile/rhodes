#include "MethodResult.h"
#include "json/JSONIterator.h"
#include "ruby/ext/rho/rhoruby.h"

#include "common/RhodesApp.h"
#include "common/StringConverter.h"

namespace rho
{
namespace apiGenerator
{

using namespace rho::json;
using namespace rho::common;

extern "C" const char* rho_webview_execute_js(const char* js, int index);
extern "C" int rho_webview_active_tab();

rho::String CMethodResult::toJSON()
{ 
    rho::String strRes = "{}";
    if ( m_ResType == eStringArray )
    {
        strRes = "[";
        for( int i = 0; i < (int)m_arStrRes.size(); i++ )
        {
            if ( i > 0 )
                strRes += ",";

            strRes += CJSONEntry::quoteValueW(m_arStrRes[i]);
        }
        
        strRes += "]";
    }else if ( m_ResType == eStringHash )
    {
        strRes = "{";

        for ( rho::Hashtable<rho::StringW, rho::StringW>::iterator it = m_hashStrRes.begin(); it != m_hashStrRes.end(); ++it)
        {
            if ( it != m_hashStrRes.begin() )
                strRes += ",";

            strRes += CJSONEntry::quoteValueW(it->first) + ":" + CJSONEntry::quoteValueW(it->second);
        }

        strRes += "}";
    }else if ( m_ResType == eString)
    {
        strRes = "{'_RhoValue':" + convertToStringA(m_strRes) + "}";
    }else if ( m_ResType == eArgError )
    {
        strRes = "{'_RhoArgError':" + CJSONEntry::quoteValueW(m_strError) + "}";
    }else if ( m_ResType == eError)
    {
        strRes = "{'_RhoRuntimeError':" + CJSONEntry::quoteValueW(m_strError) + "}";
    }

    return strRes;
}

VALUE CMethodResult::toRuby()
{
    if ( m_ResType == eStringArray )
    {
        CHoldRubyValue valArray(rho_ruby_create_array());

        for( int i = 0; i < (int)m_arStrRes.size(); i++ )
        {
            VALUE valObj = 0;
            if ( m_oRubyObjectClass )
                valObj = rho_ruby_create_object_with_id( m_oRubyObjectClass, convertToStringA(m_arStrRes[i]).c_str() );
            else
                valObj = rho_ruby_create_string( convertToStringA(m_arStrRes[i]).c_str() );
            
            rho_ruby_add_to_array( valArray, valObj );
        }
        
        return valArray;
    }else if ( m_ResType == eStringHash )
    {
        CHoldRubyValue valHash(rho_ruby_createHash());

        for ( rho::Hashtable<rho::StringW, rho::StringW>::iterator it = m_hashStrRes.begin(); it != m_hashStrRes.end(); ++it)
        {
            addStrToHash( valHash, convertToStringA(it->first).c_str(), convertToStringA(it->second).c_str() );
        }

        return valHash;
    }else if ( m_ResType == eString)
    {
        VALUE valObj = 0;
        if ( m_oRubyObjectClass )
            valObj = rho_ruby_create_object_with_id( m_oRubyObjectClass, convertToStringA(m_strRes).c_str() );
        else
            valObj = rho_ruby_create_string( convertToStringA(m_strRes).c_str() );

        return valObj;
    }else if ( m_ResType == eArgError)
    {
        rho_ruby_raise_argerror( convertToStringA(m_strError).c_str());
    }else if ( m_ResType == eError)
    {
        rho_ruby_raise_runtime( convertToStringA(m_strError).c_str());
    }

    return rho_ruby_get_NIL();
}

class CRubyCallbackResult : public rho::ICallbackObject
{
    CMethodResult m_oResult;
public:
    CRubyCallbackResult(const CMethodResult& oResult) : m_oResult(oResult){}
    ~CRubyCallbackResult()
    {
        int i = 0;
    }

    virtual unsigned long getObjectValue()
    {
        m_oResult.convertStringParamToHash();
        return m_oResult.toRuby();
    }

};

void CMethodResult::callCallback()
{
    if ( m_ResType != eNone && m_strRubyCallback.length() != 0 )
    {
        rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult( *this ), "body");

        RHODESAPP().callCallbackWithData( m_strRubyCallback, strResBody, m_strCallbackParam, true);

        m_ResType = eNone;
    }else if ( m_ResType != eNone && m_pRubyCallbackProc)
    {
        VALUE oProc = m_pRubyCallbackProc->getValue();
        rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult( *this ), "body");

        RHODESAPP().callCallbackProcWithData( oProc, strResBody, m_strCallbackParam, true);

        m_ResType = eNone;
    }else if ( m_ResType != eNone && m_strJSCallback.length() != 0 )
    {
        rho_webview_execute_js( m_strJSCallback.c_str(), rho_webview_active_tab() );
        m_ResType = eNone;
    }
}

void CMethodResult::convertStringParamToHash()
{
    if ( m_ResType == eString )
    {
        rho::Hashtable<rho::StringW, rho::StringW> resHash;
        resHash.put( convertToStringW(m_strStringParam), m_strRes);
        m_hashStrRes = resHash; m_ResType = eStringHash;
    }
}

CMethodResult::CMethodRubyValue::CMethodRubyValue(unsigned long val) : m_value(val)
{ 
    rho_ruby_holdValue(m_value); 
}

CMethodResult::CMethodRubyValue::~CMethodRubyValue()
{ 
    rho_ruby_releaseValue(m_value); 
}

void CMethodResult::setRubyCallbackProc(unsigned long oRubyCallbackProc)
{ 
    m_pRubyCallbackProc = new CMethodRubyValue(oRubyCallbackProc);
}

}
}

#include "MethodResult.h"
#include "json/JSONIterator.h"
#include "ext/rho/rhoruby.h"

#include "common/RhodesApp.h"

using namespace rho::json;

rho::String CMethodResult::toJSON()
{ 
    rho::String strRes;
    if ( m_ResType == eStringArray )
    {
        strRes = "[";
        for( int i = 0; i < m_arStrRes.size(); i++ )
        {
            if ( i > 0 )
                strRes += ",";

            strRes += CJSONEntry::quoteValue(m_arStrRes[i]);
        }
        
        strRes += "]";
    }else if ( m_ResType == eStringHash )
    {
        strRes = "{";

        for ( rho::Hashtable<rho::String, rho::String>::iterator it = m_hashStrRes.begin(); it != m_hashStrRes.end(); ++it)
        {
            if ( it != m_hashStrRes.begin() )
                strRes += ",";

            strRes += CJSONEntry::quoteValue(it->first) + ":" + CJSONEntry::quoteValue(it->second);
        }

        strRes += "}";
    }else if ( m_ResType == eString)
    {
        strRes = m_strRes;
    }else if ( m_ResType == eArgError )
    {
        strRes = "{'_RhoArgError':" + CJSONEntry::quoteValue(m_strError) + "}";
    }else if ( m_ResType == eError)
    {
        strRes = "{'_RhoRuntimeError':" + CJSONEntry::quoteValue(m_strError) + "}";
    }

    return strRes;
}

VALUE CMethodResult::toRuby()
{
    if ( m_ResType == eStringArray )
    {
        CHoldRubyValue valArray(rho_ruby_create_array());

        for( int i = 0; i < m_arStrRes.size(); i++ )
        {
            VALUE valObj = rho_ruby_create_string( m_arStrRes[i].c_str() );
            rho_ruby_add_to_array( valArray, valObj );
        }
        
        return valArray;
    }else if ( m_ResType == eStringHash )
    {
        CHoldRubyValue valHash(rho_ruby_createHash());

        for ( rho::Hashtable<rho::String, rho::String>::iterator it = m_hashStrRes.begin(); it != m_hashStrRes.end(); ++it)
        {
            addStrToHash( valHash, it->first.c_str(), it->second.c_str() );
        }

        return valHash;
    }else if ( m_ResType == eString)
    {
        return rho_ruby_create_string(m_strRes.c_str());
    }else if ( m_ResType == eArgError)
    {
        rho_ruby_raise_argerror(m_strError.c_str());
    }else if ( m_ResType == eError)
    {
        rho_ruby_raise_runtime(m_strError.c_str());
    }

    return rho_ruby_get_NIL();
}

class CRubyCallbackResult : public rho::ICallbackObject
{
    CMethodResult m_oResult;
public:
    CRubyCallbackResult(const CMethodResult& oResult) : m_oResult(oResult){}
    ~CRubyCallbackResult(){}

    virtual unsigned long getObjectValue()
    {
        m_oResult.convertStringParamToHash();
        return m_oResult.toRuby();
    }

};

void CMethodResult::callCallback()
{
    //TODO: support JSON callbacks

    if ( m_ResType != eNone && m_strRubyCallback.length() != 0 )
    {
        rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult( *this ), "body");

        RHODESAPP().callCallbackWithData( m_strRubyCallback, strResBody, m_strCallbackParam, false);

        m_ResType = eNone;
    }
}

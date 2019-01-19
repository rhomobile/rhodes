#include "MethodResult.h"

#include "common/RhodesApp.h"
#include "common/StringConverter.h"

#include "rubyext/WebView.h"

#include "MethodResultConvertor.h"
#include "RubyResultConvertor.h"
#include "JSONResultConvertor.h"
#include "js_helpers.h"

namespace rho
{
namespace apiGenerator
{

using namespace rho::json;
using namespace rho::common;

IMPLEMENT_LOGCLASS(CMethodResult, "MethodResult");

rho::String CMethodResult::toJSON()
{
    rho::String strRes = CMethodResultConvertor().toJSON(*this);
    return strRes;
}

rho::String CMethodResult::toString()
{
    switch(m_ResType) {
    case eString:
        return m_strRes;
    case eStringW:
        return convertToStringA(m_strResW);
    case eJSON:
        return m_strJSONRes;
    case eBool:
        return convertToStringA(m_bRes);
    case eInt:
        return convertToStringA(m_nRes);
    case eDouble:
        return convertToStringA(m_dRes);
    case eArgError:
    case eError:
        return "error: " + getErrorString();
    }

    return rho::String();
}

void CMethodResult::convertToType(const ETypes& eType)
{
    if ( eType == m_ResType || eType == eNone || eError == m_ResType || eArgError == m_ResType )
        return;

    switch(eType)
    {
    case eString:
        if ( m_ResType != eJSON )
        {
            m_strRes = toString();
            m_ResType = eString;
        }
        break;
    case eStringW:
        m_strResW = convertToStringW(toString());
        m_ResType = eStringW;
        break;
    case eBool:
        {
            switch(m_ResType)
            {
            case eString:
                convertFromStringA(m_strRes.c_str(), m_bRes);
                m_ResType = eBool;
                break;
#if defined(WINDOWS_PLATFORM)
            case eStringW:
                convertFromStringW(m_strResW.c_str(), m_bRes);
                m_ResType = eBool;
                break;
#endif
            case eInt:
                m_bRes = m_nRes != 0;
                m_ResType = eBool;
                break;
            case eDouble:
                m_bRes = m_dRes != 0;
                m_ResType = eBool;
                break;
            default:;
            }
        }
        break;

    case eInt:
        {
            switch(m_ResType)
            {
            case eString:
                convertFromStringA(m_strRes.c_str(), m_nRes);
                m_ResType = eInt;
                break;
#if defined(WINDOWS_PLATFORM)
            case eStringW:
                convertFromStringW(m_strResW.c_str(), m_nRes);
                m_ResType = eInt;
                break;
#endif
            case eBool:
                m_nRes = m_bRes ? 1 :0;
                m_ResType = eInt;
                break;
            default:;
            }
        }
        break;
    case eDouble:
        {
            switch(m_ResType)
            {
            case eString:
                convertFromStringA(m_strRes.c_str(), m_dRes);
                m_ResType = eDouble;
                break;
#if defined(WINDOWS_PLATFORM)
            case eStringW:
                convertFromStringW(m_strResW.c_str(), m_dRes);
                m_ResType = eDouble;
                break;
#endif
            case eBool:
                m_dRes = m_bRes ? 1 :0;
                m_ResType = eDouble;
                break;
            case eInt:
                m_dRes = static_cast<double>(m_nRes);
                m_ResType = eDouble;
                break;
            default:;
            }
        }
        break;
    default:;
    }
}


VALUE CMethodResult::toRuby(bool bForCallback/* = false*/)
{
#ifndef RHORC_NO_RUBY
    VALUE valRes = CMethodResultConvertor().toRuby(*this, bForCallback);
#else
    VALUE valRes = 0;
#endif //RHORC_NO_RUBY
    return valRes;
}

bool CMethodResult::hasCallback() const
{
    return m_strRubyCallback.length() != 0 ||
#ifndef RHORC_NO_RUBY
        m_pRubyCallbackProc ||
#endif //RHORC_NO_RUBY
        m_strJSCallback.length() != 0;

}

bool CMethodResult::isEqualCallback(CMethodResult& oResult)
{
    if (!hasCallback())
    {
        return hasCallback() == oResult.hasCallback();
    }

    if ( m_strRubyCallback.length() != 0 )
    {
        return m_strRubyCallback == oResult.m_strRubyCallback;
    }
#ifndef RHORC_NO_RUBY
    if ( m_pRubyCallbackProc )
    {
        return m_pRubyCallbackProc == oResult.m_pRubyCallbackProc;
    }
#endif
    return m_strJSCallback == oResult.m_strJSCallback;
}
    
CMethodResult::ECallbackType CMethodResult::getCallbackType()
{
    CMethodResult::ECallbackType cbType = ctNone;
    
    if ( m_strRubyCallback.length() != 0 )
    {
        cbType = ctRubyStr;
    }
#ifndef RHORC_NO_RUBY
    else if ( m_pRubyCallbackProc )
    {
        cbType = ctRubyProc;
    }
#endif
    else if ( m_strJSCallback.length() != 0 )
    {
        cbType = ctJavaScript;
    }

    return cbType;
}

void CMethodResult::callCallback()
{
    if ( m_bCollectionMode )
        return;
    
    if ( m_ResType != eNone )
    {
        ECallbackType cbType = getCallbackType();
        
        if (cbType != ctNone)
        {
            switch (cbType)
            {
#ifndef RHORC_NO_RUBY
                case ctRubyStr:
                {
                    rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult<CMethodResult>(*this), "__rho_inline");
                    
                    RHODESAPP().callCallbackWithData( m_strRubyCallback, strResBody, m_strCallbackParam, true);
                    break;
                }
                    
                case ctRubyProc:
                {
                    // CRubyCallbackResult<CMethodResult>(*this) will reset m_pRubyCallbackProc, save a copy
                    CMethodRubyValue rubyCallProcCopy(m_pRubyCallbackProc->getValue());
                    
                    VALUE oProc = m_pRubyCallbackProc->getValue();
                    rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult<CMethodResult>(*this), "body");
                    
                    // restore original value
                    m_pRubyCallbackProc = new CMethodRubyValue(rubyCallProcCopy.getValue());
                    
                    RHODESAPP().callCallbackProcWithData( oProc, strResBody, m_strCallbackParam, true);
                    break;
                }
#endif //RHORC_NO_RUBY                                    
                case ctJavaScript:
                {
                    String strRes(CMethodResultConvertor().toJSON(*this));
                    
                    String strCallback("Rho.callbackHandler( \"");
                    strCallback += m_strJSCallback;
                    strCallback += "\", {";
                    strCallback += strRes;
                    strCallback += "},\"";
                    strCallback += m_strCallbackParam + "\")";
                    
                    
                    if (callCustomJVMCallbackProvider(m_strJVMID.c_str(), strCallback.c_str()) != 1) {
                        
                        if (m_synchronousCallback)
                        {
                            rho_webview_execute_js_sync(strCallback.c_str(), m_iTabId);
                        } else
                        {
                            rho_webview_execute_js(strCallback.c_str(), m_iTabId);
                        }
                    }
                    break;
                }
                   
                default:
                    break;
            }
            m_ResType = eNone;
            m_synchronousCallback = false;
        }
    }
}

#ifndef RHORC_NO_RUBY 
CMethodResult::CMethodRubyValue::CMethodRubyValue(unsigned long val) : m_value(val)
{ 
    rho_ruby_holdValue(m_value); 
}

CMethodResult::CMethodRubyValue::~CMethodRubyValue()
{ 
    rho_ruby_releaseValue(m_value); 
}
#endif //RHORC_NO_RUBY

void CMethodResult::setRubyCallbackProc(unsigned long oRubyCallbackProc)
{
    LOG(TRACE) + "setRubyCallbackProc";
#ifndef RHORC_NO_RUBY 
    m_pRubyCallbackProc = new CMethodRubyValue(oRubyCallbackProc);
#endif //RHORC_NO_RUBY
}


void CMethodResult::setJSCallback(const rho::String& strCallback)
{
    LOG(TRACE) + "setJSCallback: " + strCallback + " without JVMID";
        
    m_strJSCallback = strCallback;
    m_iTabId = rho_webview_active_tab();
}
    

void CMethodResult::setJSCallback(const rho::String& strCallback, const rho::String& strJVMID)
{
    LOG(TRACE) + "setJSCallback: " + strCallback + " JVMID: "+strJVMID;

    m_strJSCallback = strCallback;
    m_iTabId = rho_webview_active_tab();
    m_strJVMID = strJVMID;
}
    
bool CMethodResult::isSynchronousCallback()
{
    return m_synchronousCallback;
}
void CMethodResult::setSynchronousCallback(bool executeSynchronous)
{
    m_synchronousCallback = executeSynchronous;
}

void CMethodResult::setJSON(const rho::String& res)
{
    #ifdef RHO_DEBUG
    CJSONEntry jsonValidator(res.c_str());
    #endif
    m_strJSONRes = res;
    setType(eJSON);
}

    
}
}

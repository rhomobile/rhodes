#include "MethodResult.h"

#include "common/RhodesApp.h"
#include "common/StringConverter.h"

#include "rubyext/WebView.h"

#include "MethodResultConvertor.h"
#include "RubyResultConvertor.h"
#include "JSONResultConvertor.h"

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
    if ( m_ResType == eString)
        return m_strRes;
    if ( m_ResType == eStringW)
        return convertToStringA(m_strResW);
    else if ( m_ResType == eJSON)
        return m_strJSONRes;
    else if ( m_ResType == eBool)
        return convertToStringA(m_bRes);
    else if ( m_ResType == eInt)
        return convertToStringA(m_nRes);
    else if ( m_ResType == eDouble)
        return convertToStringA(m_dRes);

    return rho::String();
}

void CMethodResult::convertToType(ETypes eType)
{
    if ( eType == m_ResType || eType == eNone )
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
            }
        }
        break;
    }
}

VALUE CMethodResult::toRuby(bool bForCallback/* = false*/)
{
    VALUE valRes = CMethodResultConvertor().toRuby(*this, bForCallback);
    return valRes;
}

bool CMethodResult::hasCallback()
{
    return m_strRubyCallback.length() != 0 || m_pRubyCallbackProc || m_strJSCallback.length() != 0;

}

bool CMethodResult::isEqualCallback(CMethodResult& oResult)
{
    if (!hasCallback())
        return hasCallback() == oResult.hasCallback();

    if ( m_strRubyCallback.length() != 0 )
        return m_strRubyCallback == oResult.m_strRubyCallback;

    if ( m_pRubyCallbackProc )
        return m_pRubyCallbackProc == oResult.m_pRubyCallbackProc;

    return m_strJSCallback == oResult.m_strJSCallback;
}

void CMethodResult::callCallback()
{
    if ( m_bCollectionMode )
        return;

    if ( m_ResType != eNone && m_strRubyCallback.length() != 0 )
    {
        rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult<CMethodResult>(*this), "__rho_inline");

        RHODESAPP().callCallbackWithData( m_strRubyCallback, strResBody, m_strCallbackParam, true);

        m_ResType = eNone;
    }else if ( m_ResType != eNone && m_pRubyCallbackProc)
    {
        VALUE oProc = m_pRubyCallbackProc->getValue();
        rho::String strResBody = RHODESAPP().addCallbackObject( new CRubyCallbackResult<CMethodResult>(*this), "body");

        RHODESAPP().callCallbackProcWithData( oProc, strResBody, m_strCallbackParam, true);

        m_ResType = eNone;
    }else if ( m_ResType != eNone && m_strJSCallback.length() != 0 )
    {
        String strRes(CMethodResultConvertor().toJSON(*this));

        String strCallback("Rho.callbackHandler( \"");
        strCallback += m_strJSCallback;
        strCallback += "\", {";
        strCallback += strRes;
        strCallback += "})";

        rho_webview_execute_js(strCallback.c_str(), m_iTabId);

        m_ResType = eNone;
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
    LOG(TRACE) + "setRubyCallbackProc";

    m_pRubyCallbackProc = new CMethodRubyValue(oRubyCallbackProc);
}

void CMethodResult::setJSCallback(const rho::String& strCallback)
{
    LOG(TRACE) + "setJSCallback: " + strCallback;

    m_strJSCallback = strCallback;
    m_iTabId = rho_webview_active_tab();
}

}
}

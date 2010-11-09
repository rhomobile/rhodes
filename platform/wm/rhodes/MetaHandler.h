#pragma once

#include "logging/RhoLog.h"

class CMetaHandler
{
    DEFINE_LOGCLASS;

    class CBatteryEvent
    {
        CStringW m_strCallback;

    public:
        CBatteryEvent(){}
        CBatteryEvent(const CStringW& strCallback) : m_strCallback(strCallback){}

        CStringW makeJSCallback();
    };

    HRESULT m_hResult;
    CBatteryEvent m_oBatteryEvent;

public:
    CMetaHandler(CComPtr<IWebBrowser2> pIWebBrowser2);
    ~CMetaHandler(void);
private:
    void readMetaTags(CComPtr<IWebBrowser2> pIWebBrowser2);
    void handleMetaTag(BSTR bstrHttpEquiv, BSTR bstrContent);
    int  checkHResult(HRESULT res, const char* file, int line, const char* msg );
    void checkEvents();
};

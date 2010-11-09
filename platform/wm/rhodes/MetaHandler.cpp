#include "StdAfx.h"
#include "MetaHandler.h"
#include "MainWindow.h"

#if defined (_WIN32_WCE)
#include <webvw.h>
#if _WIN32_WCE > 0x501
#define IHTMLDocument3 IPIEHTMLDocument3
#define IHTMLElement IPIEHTMLElement2
#else
#define IHTMLDocument3 IPIEHTMLDocument2
#define IHTMLElement IPIEHTMLElement
#endif
#define IHTMLElementCollection IPIEHTMLElementCollection

#endif

extern "C" HWND getMainWnd();

IMPLEMENT_LOGCLASS(CMetaHandler,"MetaHandler");

CMetaHandler::CMetaHandler(CComPtr<IWebBrowser2> pIWebBrowser2)
{
    readMetaTags(pIWebBrowser2);
    checkEvents();
}

CMetaHandler::~CMetaHandler(void)
{
}

CStringW CMetaHandler::CBatteryEvent::makeJSCallback()
{
    if ( m_strCallback.GetLength() == 0 )
        return CStringW();

    CStringW strParam1, strParam2, strParam3;
#ifdef _WIN32_WCE
    SYSTEM_POWER_STATUS_EX2 pwrstat = {0};
    if (!GetSystemPowerStatusEx2(&pwrstat,sizeof(pwrstat),FALSE))
    {
        LOG(ERROR) + "GetSystemPowerStatusEx2 failed.";
        return CStringW();
    } 
    strParam1 = pwrstat.ACLineStatus == AC_LINE_BACKUP_POWER ? L"1" : L"0";
    strParam2.Format(L"%d", pwrstat.BatteryLifePercent);
    strParam3.Format(L"%d", pwrstat.BackupBatteryLifePercent);
#else
    SYSTEM_POWER_STATUS pwrstat = {0};
    if (!GetSystemPowerStatus(&pwrstat))
    {
        LOG(ERROR) + "GetSystemPowerStatus failed.";
        return CStringW();
    }
    strParam1 = L"0";
    strParam2.Format(L"%d", pwrstat.BatteryLifePercent);
    strParam3.Format(L"%c", '0');

#endif

    CStringW strRes;
    strRes.Format(m_strCallback, strParam1.GetString(), strParam2.GetString(), strParam3.GetString());

    if ( strRes.Find(L"BatteryEvent:url(\'") == 0 )
    {
        strRes = strRes.Mid(18, (strRes.GetLength()-18-2));
    }

    return strRes;
}

void CMetaHandler::checkEvents()
{
    CStringW strJS = m_oBatteryEvent.makeJSCallback();
    if (strJS.GetLength() > 0 )
        ::PostMessage( getMainWnd(), WM_COMMAND, IDM_NAVIGATE, (LPARAM)_tcsdup(strJS) );
}

void CMetaHandler::handleMetaTag(BSTR bstrHttpEquiv, BSTR bstrContent)
{
    if (!bstrHttpEquiv || !*bstrHttpEquiv)
        return;

    if (_wcsicmp(bstrHttpEquiv, L"Battery") == 0)
    {
        m_oBatteryEvent = CBatteryEvent(bstrContent);       
    }
}

int CMetaHandler::checkHResult(HRESULT res, const char* file, int line, const char* msg )
{
    m_hResult = res;
    if (FAILED(res))
    {
        //LOG(ERROR) + "Error Code:" + res + ";Method: " + msg + ";Line: " + line; 
//        rho_ruby_raise_runtime("Calendar method failed.");
        return 0;
    }

    return 1;
}

#define CHECK(res, ptr) {if(!checkHResult(res, __FILE__, __LINE__, #res ) || !ptr) break;}
#define CHECKC(res, ptr) {if(!checkHResult(res, __FILE__, __LINE__, #res ) || !ptr) continue;}

void CMetaHandler::readMetaTags(CComPtr<IWebBrowser2> pIWebBrowser2)
{
    do
    {
        m_oBatteryEvent = CBatteryEvent();

        CComPtr<IDispatch> pDispDoc;
        CHECK(pIWebBrowser2->get_Document(&pDispDoc), pDispDoc);

        CComPtr<IHTMLDocument3> pDoc;
        CHECK(pDispDoc.QueryInterface(&pDoc), pDoc);

#if defined(_WIN32_WCE) && _WIN32_WCE <= 0x501
/*
        DISPID id = 0;
        OLECHAR* szMember = L"body";
        CHECK( pDispDoc->GetIDsOfNames(IID_NULL, &szMember, 1, LOCALE_USER_DEFAULT, &id), 1);

        CComVariant varResult;
        varResult.vt = VT_DISPATCH;
        DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};

        CHECK(pDoc->Invoke(id, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &dispparamsNoArgs, &varResult, NULL, NULL), varResult.pdispVal);
        CComPtr<IDispatch> dispHead = varResult.pdispVal;
        CComPtr<IHTMLElement> pHead;
        CHECKC( dispHead.QueryInterface(&pHead), pHead);

        CComPtr<IHTMLElementCollection> pColl;
        CHECK(pHead->get_children(&pColl), pColl);

        long size = 0;
        CHECK(pColl->get_length(&size), size);

        for( long i = 0; i < size; i++)
        {
            CComVariant index = i;
            CComPtr<IDispatch> dispMeta;
            CHECKC(pColl->item(index, index, &dispMeta), dispMeta);

            CComPtr<IHTMLElement> pMeta;
            CHECKC( dispMeta.QueryInterface(&pMeta), pMeta);

            CComBSTR bodyValue;
            CHECKC( pMeta->get_innerHTML(&bodyValue), bodyValue);
        }*/
#else
        CComPtr<IHTMLElementCollection> pColl;
        CHECK(pDoc->getElementsByTagName(CComBSTR("meta"), &pColl), pColl);

        long size = 0;
        CHECK(pColl->get_length(&size), size);

        for( long i = 0; i < size; i++)
        {
            CComVariant index = i;
            CComPtr<IDispatch> dispMeta;
            CHECKC(pColl->item(index, index, &dispMeta), dispMeta);

#ifndef _WIN32_WCE
            CComPtr<IHTMLMetaElement> pMeta;
            CHECKC( dispMeta.QueryInterface(&pMeta), pMeta);

            CComBSTR bstrHttpEquiv;
            CHECKC( pMeta->get_httpEquiv(&bstrHttpEquiv), bstrHttpEquiv);

            CComBSTR bstrContent;
            CHECKC( pMeta->get_content(&bstrContent), bstrContent);

            handleMetaTag(bstrHttpEquiv, bstrContent);
#else

            CComPtr<IHTMLElement> pMeta;
            CHECKC( dispMeta.QueryInterface(&pMeta), pMeta);

            CComBSTR bstrHttpEquiv = L"http-equiv";
            CComVariant varHttpEquiv;
            CHECKC( pMeta->getAttribute(bstrHttpEquiv, 0, &varHttpEquiv), 1);
            CComBSTR bstrContent = L"content";
            CComVariant varContent;
            CHECKC( pMeta->getAttribute(bstrContent, 0, &varContent), 1);

            handleMetaTag(varHttpEquiv.vt == VT_BSTR ? varHttpEquiv.bstrVal : 0, varContent.vt == VT_BSTR ? varContent.bstrVal : 0);
#endif
        }
#endif
    }while(false);
}

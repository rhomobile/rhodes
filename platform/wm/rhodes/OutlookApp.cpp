#include "StdAfx.h"
#include "OutlookApp.h"

#if defined( _WIN32_WCE ) && !defined( OS_PLATFORM_CE )
#define INITGUID
#include <pimstore.h>
#endif //_WIN32_WCE

COutlookApp* COutlookApp::m_pInstance = 0;
IMPLEMENT_LOGCLASS(COutlookApp,"OutlookApp");

/*static*/ COutlookApp* COutlookApp::Create()
{
    if ( m_pInstance ) 
        return m_pInstance;

    m_pInstance = new COutlookApp();
    return m_pInstance;
}

/*static*/ void COutlookApp::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

COutlookApp::COutlookApp(void)
{
    HRESULT hr = CoInitializeEx( NULL, 0);

#if defined( _WIN32_WCE ) && !defined( OS_PLATFORM_CE )
    IUnknown* pUnknown = NULL;
    hr = CoCreateInstance(__uuidof(Application),//CLSID_Application, 
        NULL, CLSCTX_INPROC_SERVER, 
        IID_IUnknown, 
        (void **)&pUnknown);
    if (hr != S_OK) {
        // CoCreateInstance failed.
        LOG(ERROR) + "CoCreateInstance failed.";
        return;
    }

    //IID_IPOutlookApp
    hr = pUnknown->QueryInterface(__uuidof(IPOutlookApp), (void**)&m_outlookApp); 
    if (hr != S_OK) {
        // QueryInterface failed.
        LOG(ERROR) + "QueryInterface failed.";
        return;
    }

    hr = m_outlookApp->Logon(NULL);
    if (hr != S_OK) {
        // Logon failed.
        LOG(ERROR) + "Logon failed.";
        m_outlookApp->Release();
        return;
    }
#endif //_WIN32_WCE
}

COutlookApp::~COutlookApp(void)
{
#if defined( _WIN32_WCE ) && !defined( OS_PLATFORM_CE )
	if(m_outlookApp) 
		m_outlookApp->Release();
#endif //_WIN32_WCE

    m_outlookApp = 0;
	CoUninitialize();
}

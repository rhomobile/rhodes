/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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

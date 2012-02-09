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

#include "ExtManager.h"
#include "rubyext/WebView.h"

#if defined (WINDOWS_PLATFORM)
extern "C" HWND getMainWnd();
extern "C" HINSTANCE rho_wmimpl_get_appinstance();
#endif

namespace rho {
namespace common {

IMPLEMENT_LOGCLASS(CExtManager, "ExtManager");

void CExtManager::registerExtension(const String& strName, IRhoExtension* pExt)
{
    m_hashExtensions.put(strName, pExt);
}

IRhoExtension* CExtManager::getExtByName(const String& strName)
{
    return m_hashExtensions.get(strName);
}

CRhoExtData CExtManager::makeExtData()
{
    CRhoExtData oData;
#if defined (OS_WINDOWS)
    oData.m_hWnd = getMainWnd();
    oData.m_hInstance = rho_wmimpl_get_appinstance();
#endif

    oData.m_iTabIndex = rho_webview_active_tab();
    return oData;
}

void CExtManager::onBeforeNavigate()
{
    for ( HashtablePtr<String, IRhoExtension*>::iterator it = m_hashExtensions.begin(); it != m_hashExtensions.end(); ++it )
    {
        (it->second)->onBeforeNavigate( makeExtData() );
    }
}

void CExtManager::close()
{
   m_hashExtensions.clear();
}

void CExtManager::onUnhandledProperty( const wchar_t* pModuleName, const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData )
{
    rho::common::IRhoExtension* pExt = getExtByName( rho::common::convertToStringA(pModuleName) );
    if (pExt)
        pExt->onSetProperty( pName, pValue, oExtData );
}

void CExtManager::executeRuby( const char* szMethod, const char* szParam)
{
}

void CExtManager::executeRubyCallback( const char* szCallback, const char* szCallbackBody, const char* szCallbackData)
{
}

void CExtManager::navigate(const wchar_t* szUrl)
{
}

extern "C" unsigned long rb_require(const char *fname);

void CExtManager::requireRubyFile( const char* szFilePath )
{
    rb_require(szFilePath);
}

} //namespace common
} //namespace rho


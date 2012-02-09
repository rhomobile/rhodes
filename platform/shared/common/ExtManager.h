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

#ifndef __EXT_MANAGER__H__
#define __EXT_MANAGER__H__

#ifdef __cplusplus

#include "logging/RhoLog.h"

#if defined(WINDOWS_PLATFORM)

namespace rho {
namespace common {

struct CRhoExtData
{
	void*	    m_hWnd;
	void*		m_hInstance;
	int			m_iTabIndex;
};

struct IRhoExtension 
{
    virtual void onSetPropertiesData( const wchar_t* pData, const CRhoExtData& oExtData ){}
    virtual void onSetProperty( const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData ){}
    virtual void onBeforeNavigate(const CRhoExtData& oExtData){}
};

struct IRhoExtManager
{
    virtual void onUnhandledProperty( const wchar_t* pModuleName, const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData ) = 0;
    virtual void executeRuby( const char* szMethod, const char* szParam) = 0;
    virtual void executeRubyCallback( const char* szCallback, const char* szCallbackBody, const char* szCallbackData) = 0;
    virtual void requireRubyFile( const char* szFilePath ) = 0;

    virtual void navigate(const wchar_t* szUrl) = 0;
};

class CExtManager : public IRhoExtManager
{
	DEFINE_LOGCLASS;

	HashtablePtr<String, IRhoExtension*> m_hashExtensions;

public:
    void registerExtension(const String& strName, IRhoExtension* pExt);

    IRhoExtension* getExtByName(const String& strName);
    void onBeforeNavigate();

    CRhoExtData makeExtData();
    void close();

    //IRhoExtManager
    virtual void onUnhandledProperty( const wchar_t* pModuleName, const wchar_t* pName, const wchar_t* pValue, const CRhoExtData& oExtData );
    virtual void executeRuby( const char* szMethod, const char* szParam);
    virtual void executeRubyCallback( const char* szCallback, const char* szCallbackBody, const char* szCallbackData);
    virtual void navigate(const wchar_t* szUrl);
    virtual void requireRubyFile( const char* szFilePath );

};

} //namespace common
} //namespace rho

#else //WINDOWS_PLATFORM
class CExtManager
{
public:
    void close(){}
};

#endif

#endif //__cplusplus

#endif //__EXT_MANAGER__H__

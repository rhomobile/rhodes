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

#ifndef _RHOATLLOG_H_
#define _RHOATLLOG_H_

#ifdef _WIN32_WINCE
#include <stdlib.h>
#else
#include <stdio.h>
#include <stdarg.h>
#endif

#include "common/RhoDefs.h"

extern "C" void rhoPlainLogArg(const char* file, int line, int severity, const char* szCategory,
                 const char* format, va_list ap );
extern "C" void rhoPlainLogArgW(const char* file, int line, int severity, const char* szCategory,
                 const wchar_t* format, va_list ap );
extern "C" int rhoPlainLog(const char* file, int line, int severity, const char* szCategory,
                  const char* msg );

namespace rho {

#if defined (RHO_DEBUG) && RHO_STRIP_LOG <= L_TRACE
class CTraceFileAndLineInfo
{
public:
	CTraceFileAndLineInfo(const char *pszFileName, int nLineNo)
		: m_pszFileName(pszFileName), m_nLineNo(nLineNo)
	{}

#ifndef _WIN32_WCE
#pragma warning(push)
#pragma warning(disable : 4793)
	void __cdecl operator()(unsigned long dwCategory, unsigned int nLevel, const char *pszFmt, ...) const
	{
		va_list ptr; va_start(ptr, pszFmt);
        rhoPlainLogArg( m_pszFileName, m_nLineNo, 0, "ATLTrace", pszFmt, ptr );
		//ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, dwCategory, nLevel, pszFmt, ptr);
		va_end(ptr);
	}
#pragma warning(pop)
#endif // _WIN32_WCE

#pragma warning(push)
#pragma warning(disable : 4793)
	void __cdecl operator()(unsigned long dwCategory, unsigned int nLevel, const wchar_t *pszFmt, ...) const
	{
		va_list ptr; va_start(ptr, pszFmt);
        rhoPlainLogArgW( m_pszFileName, m_nLineNo, 0, "ATLTrace", pszFmt, ptr );
		//ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, dwCategory, nLevel, pszFmt, ptr);
		va_end(ptr);
	}
#pragma warning(pop)

#ifndef _WIN32_WCE
#pragma warning(push)
#pragma warning(disable : 4793)
	void __cdecl operator()(const char *pszFmt, ...) const
	{
		va_list ptr; va_start(ptr, pszFmt);
        rhoPlainLogArg( m_pszFileName, m_nLineNo, 0, "ATLTrace", pszFmt, ptr );
		//ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, atlTraceGeneral, 0, pszFmt, ptr);
		va_end(ptr);
	}
#pragma warning(pop)
#endif // _WIN32_WCE

#pragma warning(push)
#pragma warning(disable : 4793)
	void __cdecl operator()(const wchar_t *pszFmt, ...) const
	{
		va_list ptr; va_start(ptr, pszFmt);
        rhoPlainLogArgW( m_pszFileName, m_nLineNo, 0, "ATLTrace", pszFmt, ptr );

		//ATL::CTrace::s_trace.TraceV(m_pszFileName, m_nLineNo, atlTraceGeneral, 0, pszFmt, ptr);
		va_end(ptr);
	}
#pragma warning(pop)

private:
	/* unimplemented */
	CTraceFileAndLineInfo &__cdecl operator=(const CTraceFileAndLineInfo &right);

	const char *const m_pszFileName;
	const int m_nLineNo;
};

#define ATLTRACE rho::CTraceFileAndLineInfo(__FILE__, __LINE__)
#define ATLTRACE2 ATLTRACE

#else

#define ATLTRACE
#define ATLTRACE2

#endif  // RHO_DEBUG

}

#define ASSERT(exp) ( (void) ((exp) || rhoPlainLog(__FILE__, __LINE__, 4, "", #exp )) )
#define VERIFY(exp) ASSERT(exp)

#endif //_RHOATLLOG_H_
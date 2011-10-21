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

#include "RhoLog.h"

using namespace rho;

extern "C"{
const char* _rawDefaultCategory = "";

static common::CMutex g_plainBufferLock;
static char g_plainBuffer[2000];

void rhoPlainLogArg(const char* file, int line, LogSeverity severity, const char* szCategory,
                    const char* format, va_list ap ){
    LogMessage oLogMsg(file, line, severity, LOGCONF(), LogCategory(szCategory) );

    if ( oLogMsg.isEnabled() )
    {
        common::CMutexLock oLock(g_plainBufferLock);

        int buflen = sizeof(g_plainBuffer)-1;
        int len = vsnprintf(g_plainBuffer, buflen, format, ap);

        if (len < 0 || len >= buflen){
#ifdef __SYMBIAN32__
            len = buflen - 1;
#else
            len = buflen;
#endif
        }
        g_plainBuffer[len] = 0;

        oLogMsg + g_plainBuffer;
    }

}

#ifdef OS_ANDROID
#define rhoPlainLogArgW rhoPlainLogArg
#else
void rhoPlainLogArgW(const char* file, int line, int severity, const char* szCategory,
                     const wchar_t* format, va_list ap )
{
    LogMessage oLogMsg(file, line, severity, LOGCONF(), LogCategory(szCategory) );

    if ( oLogMsg.isEnabled() )
    {
        common::CMutexLock oLock(g_plainBufferLock);

        int buflen = sizeof(g_plainBuffer)/2-1;
        wchar_t* buf = (wchar_t*)g_plainBuffer;

        int len = vswnprintf(buf, buflen, format, ap);

        if (len < 0 || len >= buflen){
#ifdef OS_SYMBIAN
            len = buflen - 1;
#else
            len = buflen;
#endif
        }
        buf[len] = 0;

        oLogMsg + buf;
    }
}
#endif // OS_ANDROID

void rhoPlainLogVar(const char* file, int line, LogSeverity severity, const char* szCategory,
                 const char* format, ... ){

    va_list ap;
    va_start(ap, format);
    rhoPlainLogArg(file,line,severity,szCategory,format,ap);
    va_end(ap);
}

int rhoPlainLog(const char* file, int line, LogSeverity severity, const char* szCategory,
                  const char* msg ){

    LogMessage(file, line, severity, LOGCONF(), LogCategory(szCategory) ) + msg;

    return 1;
}

int rhoPlainLogData(const char* file, int line, LogSeverity severity, const char* szCategory,
				const void* data, int len ){

	LogMessage oMsg(file, line, severity, LOGCONF(), LogCategory(szCategory) );
	oMsg.addRawString( static_cast<const char*>(data),len);
	return 1;
}

}

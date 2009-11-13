#include "RhoLog.h"

extern "C"{
const char* __rawDefaultCategory = "";

static rho::common::CMutex g_plainBufferLock;
static char g_plainBuffer[2000];

void rhoPlainLogArg(const char* file, int line, LogSeverity severity, const char* szCategory,
                    const char* format, va_list ap ){
    rho::LogMessage oLogMsg(file, line, severity, LOGCONF(), rho::LogCategory(szCategory) );

    if ( oLogMsg.isEnabled() )
    {
        rho::common::CMutexLock oLock(g_plainBufferLock);

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
    rho::LogMessage oLogMsg(file, line, severity, LOGCONF(), rho::LogCategory(szCategory) );

    if ( oLogMsg.isEnabled() )
    {
        rho::common::CMutexLock oLock(g_plainBufferLock);

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

    rho::LogMessage(file, line, severity, LOGCONF(), rho::LogCategory(szCategory) ) + msg;

    return 1;
}

int rhoPlainLogData(const char* file, int line, LogSeverity severity, const char* szCategory,
				const void* data, int len ){
		
	rho::LogMessage oMsg(file, line, severity, LOGCONF(), rho::LogCategory(szCategory) );
	oMsg.addRawString( static_cast<const char*>(data),len);	
	return 1;
}
	
}

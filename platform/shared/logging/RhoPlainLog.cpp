#include "RhoLog.h"

using namespace rho;

extern "C"{
const char* __rawDefaultCategory = "";

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

int rhoPlainLog_Secure(const char* file, int line, LogSeverity severity, const char* szCategory,
                  const char* msg )
{
    Vector<String>& arSecure = LOGCONF().getExcludeAttribs();
    if ( arSecure.size() == 0 )
    {
        LogMessage(file, line, severity, LOGCONF(), LogCategory(szCategory) ) + msg;
    }else
    {
        String strMsg = msg;
        for ( int i = 0; i < strMsg.length(); i++ )
        {
            rho::boolean bFound = false;
            for ( int j = 0; j < arSecure.size(); j++ )
            {
                const String& strExclude = arSecure.elementAt(j);
                if ( strncmp(strMsg.c_str() + i, strExclude.c_str(), strExclude.length()) == 0 )
                {
                    rho::boolean bSlash = false;
                    int nRemoveStart = i + strExclude.length(); 
                    for ( int nFill = nRemoveStart; nFill < strMsg.length(); nFill++ )
                    {
                        if ( strMsg[nFill] == '\\' )
                            bSlash = true;
                        else
                        {
                            if ( strMsg[nFill] == '"' && !bSlash )
                            {
                                strMsg.erase(nRemoveStart, nFill-nRemoveStart );
                                i += strExclude.length();
                                bFound = true;
                                break;
                            }

                            bSlash = false;
                        }
                    }
                }
                if ( bFound )
                    break;
            }
        }

        LogMessage(file, line, severity, LOGCONF(), LogCategory(szCategory) ) + strMsg;
    }

    return 1;
}

int rhoPlainLogData(const char* file, int line, LogSeverity severity, const char* szCategory,
				const void* data, int len ){
		
	LogMessage oMsg(file, line, severity, LOGCONF(), LogCategory(szCategory) );
	oMsg.addRawString( static_cast<const char*>(data),len);	
	return 1;
}
	
}

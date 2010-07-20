#include "common/RhoPort.h"
#include "common/RhoStd.h"
#ifdef OS_WINDOWS
#include <atlstr.h>
#endif

extern "C" const char* rho_native_rhopath() 
{
    static rho::String m_strRootPath;

    if ( m_strRootPath.length() == 0 )
    {
        char rootpath[MAX_PATH];
        int len;
        if ( (len = GetModuleFileNameA(NULL,rootpath,MAX_PATH)) == 0 )
            strcpy(rootpath,".");
        else
        {
            while( !(rootpath[len] == '\\'  || rootpath[len] == '/') )
              len--;
            rootpath[len+1]=0;
        }

        m_strRootPath = rootpath;
        m_strRootPath += "rho/";
    }

    return m_strRootPath.c_str();
}

int rho_testvprintf(const char * fmt, va_list args)
{
#ifdef OS_WINDOWS
    CAtlStringA str;
    str.FormatV(fmt,args);
    OutputDebugStringA(str);
#endif //OS_WINDOWS
    return vfprintf(stdout,fmt, args);
}

int rho_testprintf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int nres = rho_testvprintf(fmt, args);
    va_end(args);

    return nres;
}

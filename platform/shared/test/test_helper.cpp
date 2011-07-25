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

#include "common/RhoPort.h"
#include "common/RhoStd.h"
#include "common/RhodesApp.h"
#ifdef OS_WINDOWS
#include <atlstr.h>
#endif

#ifdef OS_WINDOWS		
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

rho::StringW rho::common::CRhodesApp::getAppNameW()
{
    return L"Rhodes";
}

#endif

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

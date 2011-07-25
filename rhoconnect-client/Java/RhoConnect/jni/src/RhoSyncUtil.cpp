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

//#include <android/native_activity.h>

#include "rhodes/JNIRhodes.h"

#include "logging/RhoLogConf.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSyncUtil"

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
//    JNIEnv *env = jnienv();
//    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
//    if (!cls) return;
//    jmethodID mid = getJNIClassStaticMethod(env, cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
//    if (!mid) return;
//    jhstring objFolderPath = rho_cast<jhstring>(szFolderPath);
//    env->CallStaticVoidMethod(cls, mid, objFolderPath.get());
}

rho::String rho_sysimpl_get_phone_id()
{
    return "";
}

RHO_GLOBAL void rho_free_callbackdata(void* pData)
{
}

RHO_GLOBAL int rho_net_ping_network(const char* szHost)
{
    RAWTRACE1("Ping host: %s", szHost);
    return 1;
}

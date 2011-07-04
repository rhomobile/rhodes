/*
 ============================================================================
 Author     : Alexey Tikhvinsky
 Copyright  : Copyright (C) 2011 Rhomobile (http://www.rhomobile.com).
              All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */

#include <android/native_activity.h>

#include "rhodes/JNIRhodes.h"

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

RHO_GLOBAL rho::String rho_sysimpl_get_phone_id()
{
    return "";
}

RHO_GLOBAL void rho_free_callbackdata(void* pData)
{
}

RHO_GLOBAL int rho_net_ping_network(const char* szHost)
{
    return 1;
}

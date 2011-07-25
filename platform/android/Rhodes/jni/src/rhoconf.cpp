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

#include "rhodes/jni/com_rhomobile_rhodes_RhoConf.h"

#include "rhodes/JNIRhodes.h"

#include <common/RhoConf.h>

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhoConf_getString
  (JNIEnv *, jclass, jstring name)
{
    return rho_cast<jhstring>(RHOCONF().getString(rho_cast<std::string>(name).c_str()).c_str()).release();
}

RHO_GLOBAL jint JNICALL Java_com_rhomobile_rhodes_RhoConf_getInt
  (JNIEnv *, jclass, jstring name)
{
    return RHOCONF().getInt(rho_cast<std::string>(name).c_str());
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhoConf_getBool
  (JNIEnv *, jclass, jstring name)
{
    return RHOCONF().getBool(rho_cast<std::string>(name).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoConf_setString
  (JNIEnv *, jclass, jstring name, jstring value)
{
    RHOCONF().setString(rho_cast<std::string>(name).c_str(), rho_cast<std::string>(value), false);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoConf_setInt
  (JNIEnv *, jclass, jstring name, jint value)
{
    RHOCONF().setInt(rho_cast<std::string>(name).c_str(), value, false);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhoConf_setBoolean
  (JNIEnv *, jclass, jstring name, jboolean value)
{
    RHOCONF().setBool(rho_cast<std::string>(name).c_str(), value, false);
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhoConf_isExist
  (JNIEnv *, jclass, jstring name)
{
    return RHOCONF().isExist(rho_cast<std::string>(name).c_str());
}


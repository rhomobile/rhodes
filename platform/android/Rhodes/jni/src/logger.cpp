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

#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_Logger.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Logger"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_E
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_ERROR(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_W
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_ERROR(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_I
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_INFO(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_D
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWLOGC_INFO(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_T
  (JNIEnv *env, jclass, jstring tag, jstring msg)
{
    RAWTRACEC(rho_cast<std::string>(env, tag).c_str(), rho_cast<std::string>(env, msg).c_str());
}


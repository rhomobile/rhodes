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

#include "statistic/RhoProfiler.h"

#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_Logger.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Logger"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_E
  (JNIEnv *env, jclass, jstring jTag, jstring jMsg)
{
    //RAWTRACE("Error");
    const char *tag = env->GetStringUTFChars(jTag, 0);
    const char *msg = env->GetStringUTFChars(jMsg, 0);
    RAWLOGC_ERROR(tag, msg);
    env->ReleaseStringUTFChars(jTag, tag);
    env->ReleaseStringUTFChars(jMsg, msg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_W
  (JNIEnv *env, jclass, jstring jTag, jstring jMsg)
{
    //RAWTRACE("Warning");
    const char *tag = env->GetStringUTFChars(jTag, 0);
    const char *msg = env->GetStringUTFChars(jMsg, 0);
    RAWLOGC_WARNING(tag, msg);
    env->ReleaseStringUTFChars(jTag, tag);
    env->ReleaseStringUTFChars(jMsg, msg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_I
  (JNIEnv *env, jclass, jstring jTag, jstring jMsg)
{
    //RAWTRACE("Info");
    const char *tag = env->GetStringUTFChars(jTag, 0);
    const char *msg = env->GetStringUTFChars(jMsg, 0);
    RAWLOGC_INFO(tag, msg);
    env->ReleaseStringUTFChars(jTag, tag);
    env->ReleaseStringUTFChars(jMsg, msg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_D
  (JNIEnv *env, jclass, jstring jTag, jstring jMsg)
{
#ifdef _DEBUG
    const char *tag = env->GetStringUTFChars(jTag, 0);
    const char *msg = env->GetStringUTFChars(jMsg, 0);
    RAWTRACEC(tag, msg);
    env->ReleaseStringUTFChars(jTag, tag);
    env->ReleaseStringUTFChars(jMsg, msg);
#endif
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_T
  (JNIEnv *env, jclass, jstring jTag, jstring jMsg)
{
    //RAWTRACE("Trace");
    const char *tag = env->GetStringUTFChars(jTag, 0);
    const char *msg = env->GetStringUTFChars(jMsg, 0);
    RAWTRACEC(tag, msg);
    env->ReleaseStringUTFChars(jTag, tag);
    env->ReleaseStringUTFChars(jMsg, msg);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_profStart
  (JNIEnv *env, jclass, jstring tag)
{
    PROF_START(rho_cast<std::string>(env, tag).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_Logger_profStop
  (JNIEnv *env, jclass, jstring tag)
{
    PROF_STOP(rho_cast<std::string>(env, tag).c_str());
}




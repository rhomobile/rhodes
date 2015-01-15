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
#include "rhodes/JNIRhoRuby.h"

#include <ruby/ext/rho/rhoruby.h>
#include <api_generator/js_helpers.h>

#include <string>

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_extmanager_RhoExtManagerImpl_nativeRequireRubyFile
  (JNIEnv * env, jclass, jstring jPath)
{
    std::string path = rho_cast<std::string>(env, jPath);
    rb_require(path.c_str());
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_extmanager_RhoExtManagerImpl_nativeJSCallEntryPoint
  (JNIEnv * env, jclass, jstring jQuery)
{
    initjnienv(env);
    std::string strQuery = rho_cast<std::string>(env, jQuery);
    std::string strRes = rho::apiGenerator::js_entry_point(strQuery.c_str());
    jhstring jhRes = rho_cast<jstring>(env, strRes);
    return jhRes.release();
}

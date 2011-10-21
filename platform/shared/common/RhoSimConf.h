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

#ifndef _RHOSIMCONF_H_
#define _RHOSIMCONF_H_

#ifdef __cplusplus

namespace rho{
namespace common{

class RhoSimSettings : public RhoSettings
{
public:
    const String& getRhodesPath(){return getString("rhodes_path");}
};

extern RhoSimSettings g_RhoSimSettings;

}
}
inline rho::common::RhoSimSettings& RHOSIMCONF(){ return rho::common::g_RhoSimSettings; }

#else
#endif //__cplusplus

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

const char* rho_simconf_getRhodesPath();
const char* rho_simconf_getString(const char* name);
const int rho_simconf_getBool(const char* name);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOSIMCONF_H_

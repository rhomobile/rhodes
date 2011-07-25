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

#ifndef _RHOLOGCAT_H_
#define _RHOLOGCAT_H_

#include "common/RhoPort.h"
#include "RhoLogConf.h"

namespace rho {

class LogCategory{
    String m_strName;
public:
    LogCategory(const char* szName = "") : m_strName(szName){}

    const String& getName()const{ return m_strName; }
    bool          isEmpty()const{ return m_strName.length() == 0; }
};

}

extern rho::LogCategory __rhoCurrentCategory;

#define DEFINE_LOGCLASS static rho::LogCategory __rhoCurrentCategory;\
    static rho::LogCategory getLogCategory(){return __rhoCurrentCategory;}

#define IMPLEMENT_LOGCLASS(classname, name) \
    rho::LogCategory classname::__rhoCurrentCategory = name

#define DEFINE_BASELOGCLASS rho::LogCategory __rhoCurrentCategory;\
    rho::LogCategory getLogCategory(){return __rhoCurrentCategory;}\
    void setLogCategory(const rho::LogCategory& cat){__rhoCurrentCategory = cat;}

#endif //_RHOLOGCAT_H_

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

#pragma once

namespace rho {
namespace net{
    struct INetRequestImpl;
    struct ISSL;
}

struct ICallbackObject
{
    virtual ~ICallbackObject(){};
    virtual unsigned long getObjectValue() = 0;
};

namespace common {
struct IRhoThreadImpl;
struct IRhoCrypt;

struct IRhoClassFactory
{
    virtual ~IRhoClassFactory(void){;}

    virtual IRhoThreadImpl* createThreadImpl() = 0;
    virtual net::ISSL* createSSLEngine() = 0;

    virtual IRhoCrypt* createRhoCrypt() = 0;

    virtual net::INetRequestImpl* createNetRequestImpl() = 0;
};

}
}

rho::common::IRhoClassFactory* rho_get_RhoClassFactory();


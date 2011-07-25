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

#include <net/CURLNetRequest.h>
#include <common/PosixThreadImpl.h>

#include "rhodes/RhoClassFactory.h"
#include "rhodes/sslimpl.h"
#include "rhodes/rhocryptimpl.h"

static rho::common::CRhoClassFactory g_rhoClassFactory;
rho::common::IRhoClassFactory* rho_get_RhoClassFactory()
{
    return &g_rhoClassFactory;
}

namespace rho
{
namespace common
{

net::INetRequestImpl* CRhoClassFactory::createNetRequestImpl()
{
    return new net::CURLNetRequest();
}

IRhoThreadImpl *CRhoClassFactory::createThreadImpl()
{
    return new CPosixThreadImpl();
}

net::ISSL *CRhoClassFactory::createSSLEngine()
{
    return new net::SSLImpl();
}

IRhoCrypt *CRhoClassFactory::createRhoCrypt()
{
    return new CRhoCryptImpl;
}

} // namespace common
} // namespace rho


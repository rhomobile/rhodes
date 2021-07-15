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

#include "common/RhoPort.h"
#include "common/RhoDefs.h"
#include "common/IRhoClassFactory.h"
#ifdef OS_WINDOWS_DESKTOP
#include "SecurityTokenGeneratorImpl.h"
#include "RhoThreadImpl.h"
#include "rho/net/NetRequestImpl.h"
#define CNETREQUESTIMPL new net::CNetRequestImpl()
#define CRHOTHREADIMPL new CRhoThreadImpl()
#define CRHOCRYPTIMPL NULL
#else
#include "net/CURLNetRequest.h"
#include "common/PosixThreadImpl.h"
#include "SSLImpl.h"
#define CNETREQUESTIMPL new net::CURLNetRequest()
#define CRHOTHREADIMPL new CPosixThreadImpl()
#define CRHOCRYPTIMPL NULL
#endif
// #include "RhoCryptImpl.h"

namespace rho {
namespace common {
		
class CRhoClassFactory : public common::CBaseRhoClassFactory
{
public:
    net::INetRequestImpl* createNetRequestImpl()
    {
        return CNETREQUESTIMPL;
    }

    common::IRhoThreadImpl* createThreadImpl()
    {
        return CRHOTHREADIMPL;
    }

    net::ISSL* createSSLEngine()
    {
#ifdef OS_WINDOWS_DESKTOP
        return NULL;
#else
        return new net::SSLImpl();
#endif
    }

    const rho::common::ISecurityTokenGenerator* createSecurityTokenGenerator()
    {
#if defined (OS_WINDOWS_DESKTOP) && !defined(RHODES_VERSION_LIBRARY)
        static SecurityTokenGenerator global_generator;
        return &global_generator;
#else
        return nullptr;
#endif
    }

    IRhoCrypt* createRhoCrypt()
    {
        //TODO: createRhoCrypt
        return CRHOCRYPTIMPL;
    }
};

}
}

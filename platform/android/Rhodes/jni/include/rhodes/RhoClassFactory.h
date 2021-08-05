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

#ifndef RHO_CLASS_FACTORY_b15393f788404746b0b59f04829e14ea
#define RHO_CLASS_FACTORY_b15393f788404746b0b59f04829e14ea

#include <common/IRhoClassFactory.h>
#include <net/INetRequest.h>
#include <net/ssl.h>
#include <common/IRhoThreadImpl.h>
#include <common/IRhoCrypt.h>

#include <common/RhoMutexLock.h>
#include <common/AutoPointer.h>

#include "rhodes/sslimpl.h"
#include "common/ISecurityTokenGenerator.h"


namespace rho
{
namespace common
{

class CRhoClassFactory : public CBaseRhoClassFactory
{
public:
    //net::INetRequestImpl* createNetRequestImpl();
    IRhoThreadImpl *createThreadImpl();
    IRhoCrypt* createRhoCrypt();
    const ISecurityTokenGenerator* createSecurityTokenGenerator() override;

    CRhoClassFactory();

    net::ISSL* createSSLEngine()
    {
        if(!m_pSsl)
        {
            CMutexLock lock(m_sslMutex);
            m_pSsl = new net::SSLImpl();
        }
        return m_pSsl;
    }
    
private:    
    CMutex m_sslMutex;
    common::CAutoPtr<net::ISSL> m_pSsl;    
};

} // namespace common
} // namespace rho

#endif // RHO_CLASS_FACTORY_b15393f788404746b0b59f04829e14ea


#pragma once

#include "common/IRhoClassFactory.h"
#include "net/CURLNetRequest.h"
#include "common/PosixThreadImpl.h"
#include "net/iphone/sslimpl.h"
#include "RhoCryptImpl.h"

#include <common/RhoMutexLock.h>
#include <common/AutoPointer.h>

namespace rho {
namespace common {
		
class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    net::INetRequestImpl* createNetRequestImpl()
    {
        return new net::CURLNetRequest();
    }
    common::IRhoThreadImpl* createThreadImpl()
    {
        return new CPosixThreadImpl;
    }

    IRhoCrypt* createRhoCrypt()
    {
        return new CRhoCryptImpl();
    }

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

}
}

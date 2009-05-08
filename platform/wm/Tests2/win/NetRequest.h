#pragma once

#include "net/INetRequest.h"

namespace rho {
namespace net {

class CNetRequest : public INetRequest
{
    String m_strResponse;

public:
    CNetRequest(void);
    virtual ~CNetRequest(void);

    virtual void pullData(const String& strUrl, const String& strQuery, CNetResponse& oResponse )
    {
        if ( strUrl.length() == 0 )
            return;

        //TODO: sendRequest
    }

    virtual void pushData(const String& strUrl, const String& strQuery, const String& strBody, CNetResponse& oResponse)
    {
        if ( strUrl.length() == 0 )
            return;

        //TODO: sendRequest
    }

};

}
}

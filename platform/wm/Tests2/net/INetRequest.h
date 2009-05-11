#pragma once

#include "common/RhoStd.h"

namespace rho {
namespace net {

struct CNetResponse{
    String m_strResponse;
    int    m_nHttpCode;

    CNetResponse() : m_nHttpCode(-1){}
    boolean operator==(const CNetResponse& resp)const
    {
        return m_strResponse == resp.m_strResponse && m_nHttpCode == resp.m_nHttpCode;
    }

    boolean isOK()const{ return m_nHttpCode == 200; }
    boolean isResponseRecieved()const{ return m_nHttpCode != -1; }
};

struct INetRequest
{
    virtual ~INetRequest(void){;}

    virtual void pullData(const String& strUrl, const String& strQuery, CNetResponse& oResponse ) = 0;
    virtual void pushData(const String& strUrl, const String& strQuery, const String& strBody, CNetResponse& oResponse)=0;

};

}
}

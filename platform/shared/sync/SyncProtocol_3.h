#pragma once

#include "ISyncProtocol.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"

namespace rho {
namespace sync {

struct CSyncProtocol_3 : public ISyncProtocol
{
    String m_strContentType;

    CSyncProtocol_3()
    {
        m_strContentType = "application/json";
    }

    const String& getContentType(){ return m_strContentType; }
    virtual int getVersion(){ return 3; }

    String getLoginUrl()
    {
        return RHOCONF().getPath("syncserver") + "clientlogin";
    }

    String getLoginBody( const String& name, const String& password)
    {
        return "{\"login\":\"" + name + "\",\"password\":\"" + password + "\",\"remember_me\":1}";
    }

    String getClientCreateUrl()
    {
        return RHOCONF().getPath("syncserver") + "clientcreate";
    }

    String getClientRegisterUrl()
    {
        return RHOCONF().getPath("syncserver") + "clientregister";
    }

    String getClientRegisterBody( const String& strClientID, const String& strPin, int nPort, const String& strType, const String& strPhoneID )
    {
        return "{\"client_id\":\"" + strClientID + 
            "\",\"device_pin\":\"" + strPin +
            ( strPhoneID.length() > 0 ? "\",\"phone_id\":\"" + strPhoneID : "") +
            "\",\"device_port\":\"" + common::convertToStringA(nPort) +
            "\",\"device_type\":\"" + strType + "\"}";
    }

    String getClientResetUrl(const String& strClientID)
    {
        return RHOCONF().getPath("syncserver") + "clientreset?client_id=" + strClientID;
    }

    String getClientChangesUrl()
    {
        String strUrl = RHOCONF().getPath("syncserver");
        return strUrl.substr(0,strUrl.length()-1);
    }

    String getServerQueryUrl(const String& strAction )
    {
        String strUrl = RHOCONF().getPath("syncserver");
        if ( strAction.length() > 0 )
            strUrl = common::CFilePath::join(strUrl, strAction);
        else
            strUrl = strUrl.substr(0,strUrl.length()-1);

        return strUrl;
    }

    String getServerQueryBody(const String& strSrcName, const String& strClientID, int nPageSize )
    {
        String strQuery = "?client_id=" + strClientID + 
                "&p_size=" + common::convertToStringA(nPageSize) + "&version=3";
        
        if ( strSrcName.length() > 0 )
            strQuery += "&source_name=" + strSrcName;

        return strQuery;
    }

};

}
}

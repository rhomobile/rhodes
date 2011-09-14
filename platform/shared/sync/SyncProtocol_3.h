#pragma once

#include "ISyncProtocol.h"
#include "common/StringConverter.h"
#include "common/RhoFilePath.h"
#include "json/JSONIterator.h"

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
        return "{\"login\":" + json::CJSONEntry::quoteValue(name) + ",\"password\":" + json::CJSONEntry::quoteValue(password) + ",\"remember_me\":1}";
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
        return "{\"client_id\":" + json::CJSONEntry::quoteValue(strClientID) + 
            ",\"device_pin\":" + json::CJSONEntry::quoteValue(strPin) +
            ( strPhoneID.length() > 0 ? ",\"phone_id\":" + json::CJSONEntry::quoteValue(strPhoneID) : "") +
            ",\"device_port\":" + json::CJSONEntry::quoteValue(common::convertToStringA(nPort)) +
            ",\"device_type\":" + json::CJSONEntry::quoteValue(strType) + "}";
    }

    String getClientResetUrl(const String& strClientID)
    {
        String strUrl = RHOCONF().getPath("syncserver") + "clientreset?client_id=" + strClientID;
        String strSources = RHOCONF().getString("reset_models");
        if ( strSources.length() > 0 )
            strUrl += strSources;

        return strUrl;
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

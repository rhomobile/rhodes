package com.rho.sync;

import com.rho.RhoConf;
import com.rho.FilePath;

public class SyncProtocol_3 implements ISyncProtocol
{
    String m_strContentType;

    public SyncProtocol_3()
    {
        m_strContentType = "application/json";
    }

    public String getContentType(){ return m_strContentType; }
    public int getVersion(){ return 3; }

    public String getLoginUrl()
    {
        return RhoConf.getInstance().getPath("syncserver") + "clientlogin";
    }

    public String getLoginBody( String name, String password)
    {
        return "{\"login\":\"" + name + "\",\"password\":\"" + password + "\",\"remember_me\":1}";
    }

    public String getClientCreateUrl()
    {
        return RhoConf.getInstance().getPath("syncserver") + "clientcreate";
    }

    public String getClientRegisterUrl()
    {
        return RhoConf.getInstance().getPath("syncserver") + "clientregister";
    }

    public String getClientRegisterBody( String strClientID, String strPin, int nPort, String strType )
    {
        return "{\"client_id\":\"" + strClientID +
        	"\",\"device_pin\":\"" + strPin +
            "\",\"device_port\":\"" + nPort +
            "\",\"device_type\":\"" + strType + "\"}";
    }

    public String getClientResetUrl(String strClientID)
    {
        return RhoConf.getInstance().getPath("syncserver") + "clientreset?client_id=" + strClientID;
    }

    public String getClientChangesUrl()
    {
        String strUrl = RhoConf.getInstance().getPath("syncserver");
        return strUrl.substring(0,strUrl.length()-1);
    }

    public String getServerQueryUrl(String strAction )
    {
        String strUrl = RhoConf.getInstance().getPath("syncserver");
        if ( strAction.length() > 0 )
            strUrl = FilePath.join(strUrl, strAction);
        else
            strUrl = strUrl.substring(0,strUrl.length()-1);

        return strUrl;
    }

    public String getServerQueryBody(String strSrcName, String strClientID, int nPageSize )
    {
        String strQuery = "?client_id=" + strClientID + 
                "&p_size=" + nPageSize + "&version=3";
        
        if ( strSrcName.length() > 0 )
            strQuery += "&source_name=" + strSrcName;

        return strQuery;
    }

}

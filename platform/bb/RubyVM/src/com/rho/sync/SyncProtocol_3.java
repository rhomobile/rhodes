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
        return "{\"login\":" + JSONEntry.quoteValue(name) + ",\"password\":" + JSONEntry.quoteValue(password) + ",\"remember_me\":1}";
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
        return "{\"client_id\":" + JSONEntry.quoteValue(strClientID) +
        	",\"device_pin\":" + JSONEntry.quoteValue(strPin) +
            ",\"device_port\":\"" + nPort +
            "\",\"device_type\":" + JSONEntry.quoteValue(strType) + "}";
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

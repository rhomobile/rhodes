package com.rho.sync;
public interface ISyncProtocol
{
    public abstract String getContentType();
    public abstract int getVersion();

    public abstract String getLoginUrl();
    public abstract String getLoginBody( String name, String password);
    public abstract String getClientCreateUrl();

    public abstract String getClientRegisterUrl();
    public abstract String getClientRegisterBody( String strClientID, String strPin, int nPort, String strType );

    public abstract String getClientResetUrl(String strClientID);
    public abstract String getClientChangesUrl();

    public abstract String getServerQueryUrl(String strAction);
    public abstract String getServerQueryBody(String strSrcName, String strClientID, int nPageSize );

}

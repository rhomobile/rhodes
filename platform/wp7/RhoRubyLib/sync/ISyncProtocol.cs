using System;

namespace rho.sync
{
    public interface ISyncProtocol
    {
        String getContentType();
        int getVersion();

        String getLoginUrl();
        String getLoginBody(String name, String password);
        String getClientCreateUrl();

        String getClientRegisterUrl();
        String getClientRegisterBody(String strClientID, String strPin, int nPort, String strType);

        String getClientResetUrl(String strClientID);
        String getClientChangesUrl();

        String getServerQueryUrl(String strAction);
        String getServerQueryBody(String strSrcName, String strClientID, int nPageSize);
    }
}

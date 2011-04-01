using System;
using rho.common;

namespace rho.net
{
    public class NetResponse
    {
        private String m_strData = null;
        private int m_nRespCode;
        private String m_strCookies = "";

        //	public NetResponse( String strData ){ m_strData = strData; m_nRespCode = -1; }
        public NetResponse(String strData, int nRespCode) { m_strData = strData; m_nRespCode = nRespCode; }

        public String getCharData()
        {
            return m_strData;
        }
        public void setCharData(String strData)
        {
            m_strData = strData;
        }

        public int getDataSize()
        {
            return m_strData != null ? m_strData.length() : 0;
        }

        public int getRespCode()
        {
            return m_nRespCode;
        }

        public boolean isOK()
        {
            return m_nRespCode == 200 || m_nRespCode == 206;
        }

        public boolean isUnathorized()
        {
            return m_nRespCode == 401;
        }

        public boolean isRedirect()
        {
            return getRespCode() == 301 || getRespCode() == 302;
        }

        public boolean isSuccess()
        {
            return m_nRespCode > 0 && m_nRespCode < 400;
        }

        public void setCookies(String s)
        {
            m_strCookies = s;
        }

        public String getCookies()
        {
            return m_strCookies;
        }
    }
}

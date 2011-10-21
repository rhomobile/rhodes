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

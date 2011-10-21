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
using System.Net;
using System.Threading;
using System.ComponentModel;
using IronRuby.Builtins;
using System.Collections.Generic;
using rho.net;
using rho.common;

namespace rho.logging
{
    public class RhoLogServerSink : IRhoLogSink
    {
        private static RhoLogConf m_oLogConf;
        RhoConf RHOCONF() { return RhoConf.getInstance(); }
        CAsyncHttp m_aHttp = new CAsyncHttp(true);
        String m_addrHost = "";

        public RhoLogServerSink(RhoLogConf conf)
        {
            m_oLogConf = conf;
            m_addrHost = "http://"+RHOCONF().getString("rhologhost") + ":" + RHOCONF().getString("rhologport");
        }

        public void close()
        {
            if (m_aHttp != null)
                m_aHttp.stop(2);
        }

        private RhoLogConf getLogConf()
        {
            return m_oLogConf;
        }

        public int getCurPos()
        {
            return 0;
        }

        public void writeLogMessage(String strMsg)
        {
            IDictionary<object, object> map = new Dictionary<object, object>();
            Hash values = new Hash(map);
            values.Add(MutableString.Create("url"), MutableString.Create(m_addrHost));
            values.Add(MutableString.Create("body"), MutableString.Create(strMsg));
            RhoParams p = new RhoParams(values);
            m_aHttp.addHttpCommand(new CAsyncHttp.HttpCommand("POST", p));
        }

        public void clear()
        {

        }
    }
}

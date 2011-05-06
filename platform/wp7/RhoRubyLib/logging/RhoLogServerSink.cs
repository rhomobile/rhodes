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

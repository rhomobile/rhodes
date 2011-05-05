using System;
using rho.common;
using rho.net;
using System.Windows;
using System.Threading;
using System.ComponentModel;
using IronRuby.Builtins;
using System.Collections.Generic;

namespace rho.logging
{
    public class RhoLogSender
    {
        public static void logWorkerDoWork(object sender, DoWorkEventArgs e)
        {
            try
            {
                while (true)
                {
                    CAsyncHttp ahttp = new CAsyncHttp();
                    IDictionary<object, object> map = new Dictionary<object, object>();
                    Hash values = new Hash(map);
                    values.Add(MutableString.Create("url"), MutableString.Create("http://localhost:8000?logbegin_"+RhoLogger.flushLogItems()+"_logend"));
                    values.Add(MutableString.Create("body"), MutableString.Create(""));
                    RhoParams p = new RhoParams(values);
                    ahttp.addHttpCommand(new CAsyncHttp.HttpCommand("GET", p));
                    Thread.Sleep(2000);
                }
            }
            catch (Exception ex)
            {
            }
        }

        public static void logWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
        }
    }
}

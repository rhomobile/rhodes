using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.MessagingCE;
using System.Threading;

using rho.holder;
using rho.collections;
using rho.PrinterZebraImpl;

using Newtonsoft.Json;
using System.Diagnostics;

using PrintingService;

class Error
{
    public Int32 code { get; set; }
    public string message { get; set; }
}

namespace rho
{
namespace protocol
{
namespace exception
{
    public class ConstructException : Exception {}
}

namespace shared
{
    public interface IMethodResult
    {
        object result { get; set; }

        void set(string param);

        void set(object param);

        void setError(Int32 errCode, string errorMsg);
    }

    public class SleepMethodResult : IMethodResult
    {
        Int32 SleepMs { get; set; }

        public SleepMethodResult()
        {
            SleepMs = 0;
        }

        public SleepMethodResult(Int32 sleepTime)
        {
            SleepMs = sleepTime;
        }
        
        public object result { get; set; }

        public void set(string param) { }

        public void set(object param)
        {
            if (SleepMs > 0)
            {
                Thread.Sleep(SleepMs);
            }            
        }

        public void setError(Int32 errCode, string errorMsg)
        {
        }
    }


    public class DebugMethodResult : IMethodResult
    {
        public object result { get; set; }

        public void set(string param) {}

        public void set(object param) 
        {
            string resultJson = JsonConvert.SerializeObject(param);

            Debug.WriteLine(resultJson);
            Logger.Write("[DebugMethodResult] " + resultJson);
        }

        public void setError(Int32 errCode, string errorMsg) 
        {
            Debug.WriteLine(errorMsg.ToString());
            Logger.Write("[DebugMethodResult] Error: " + errorMsg);
        }
    }

    public class MethodResult : IMethodResult
    {
        private P2PMessageQueue m_toClientQueue;

        public object result { set; get; }

        public MethodResult(P2PMessageQueue toClientQueue)
        {
            m_toClientQueue = toClientQueue;
        }

        public void set(string param)
        {
            processAnswer(param, false);
        }

        public void set(object param)
        {
            string resultJson = JsonConvert.SerializeObject(param);
            processAnswer(resultJson, false);
        }

        private void processAnswer(string result, bool isError)
        {
            Logger.Write("call result: " + result);

            Message message = null;
            byte[] convertJson = Encoding.UTF8.GetBytes(result);
            message = new Message(convertJson, isError);
            m_toClientQueue.Send(message);
        }

        public void setError(Int32 errCode, string errorMsg)
        {
            processAnswer(errorMsg, true);
        }
    }
}
}
}
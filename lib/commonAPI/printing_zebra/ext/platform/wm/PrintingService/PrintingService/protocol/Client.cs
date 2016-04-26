using System;
using System.Collections.Generic;
using System.MessagingCE;

using rho.protocol.shared;
using rho.collections;

namespace rho
{
namespace protocol
{

    public interface IQueueCommand
    {
        string Name { get; }

        bool isNull { get; }
    }

namespace client
{
    public interface IClientMethod : IQueueCommand
    {
        string PrinterID { get; }

        object[] Params { get; }

        void setQueueForAnswer(P2PMessageQueue queue);
    }

    public class StringIntIntHashAndResultMethod : IClientMethod
    {
        public const Int32 badIntValue = -10000000;

        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public string rhoString { get; set; }

        public Int32 rhoInt { get; set; }

        public Int32 rhoInt2 { get; set; }

        public IReadOnlyDictionary<string, string> Dictinary { get; set; }

        public StringIntIntHashAndResultMethod()
        {
            rhoInt = badIntValue;
            rhoInt2 = badIntValue;
        }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[5] { rhoString, rhoInt, rhoInt2, Dictinary, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (rhoInt == badIntValue) || (rhoInt2 == badIntValue) || (Dictinary == null) || (rhoString == null);
            }
        }
    }

    public class StringStringIntIntAndResultMethod : IClientMethod
    {
        public const Int32 badIntValue = -10000000;

        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public string rhoString { get; set; }

        public string rhoString2 { get; set; }

        public Int32 rhoInt { get; set; }

        public Int32 rhoInt2 { get; set; }

        public StringStringIntIntAndResultMethod()
        {
            rhoInt = badIntValue;
            rhoInt2 = badIntValue;
        }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[5] { rhoString, rhoString2, rhoInt, rhoInt2, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (rhoInt == badIntValue) || (rhoInt2 == badIntValue) || (rhoString == null) || (rhoString2 == null);
            }
        }
    }

    public class StringListAndResultMethod : IClientMethod
    {
        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public string rhoString { get; set; }

        public IReadOnlyList<string> List { get; set; }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[3] { rhoString, List, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (rhoString == null) || (List == null);
            }
        }
    } 

    public class OnlyResultMethod : IClientMethod
    {
        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[1] { new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return true;
            }
        }
    }

    public class DictnaryAndStringAndResultMethod : IClientMethod
    {
        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public string rhoString { get; set; }

        public IReadOnlyDictionary<string, string> Dictinary { get; set; }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[3] { rhoString, Dictinary, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (Dictinary == null || (rhoString == null));
            }
        }
    }

    public class DictnaryAndResultMethod : IClientMethod
    {
        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public IReadOnlyDictionary<string, string> Dictinary { get; set; }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params 
        {
            get
            {
                return new object[2] { Dictinary, new MethodResult(m_queue) };
            }
        }

        public bool isNull 
        { 
            get
            {
                return (Dictinary == null);
            }    
        }
    }

    public class StringAndResultMethod : IClientMethod
    {
        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public string rhoString { get; set; }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[2] { rhoString, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (rhoString == null);
            }
        }
    }

    public class IntAndResultMethod : IClientMethod
    {
        public const Int32 badIntValue = -10000000;

        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public Int32 rhoInt { get; set; }

        public IntAndResultMethod()
        {
            rhoInt = badIntValue;
        }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[2] { rhoInt, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (rhoInt == badIntValue);
            }
        }
    }

    public class ListAndResultMethod : IClientMethod
    {
        private P2PMessageQueue m_queue = null;

        public string Name { get; set; }

        public string PrinterID { get; set; }

        public IReadOnlyList<string> List { get; set; }

        public void setQueueForAnswer(P2PMessageQueue queue)
        {
            m_queue = queue;
        }

        public object[] Params
        {
            get
            {
                return new object[2] { List, new MethodResult(m_queue) };
            }
        }

        public bool isNull
        {
            get
            {
                return (List == null);
            }
        }
    }   
}
}
}
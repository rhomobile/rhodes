using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace rho.db
{
    public class DBException : Exception
    {
        private int m_errorCode;
        public int ErrorCode
        {
            get { return m_errorCode; }
            set { m_errorCode = value; }
        }

        public DBException(Exception exc)
            : base("Message: " + exc.Message)
        {
            m_errorCode = 0;
	    }

        public DBException(int errorCode, string message)
            : base(message)
        {
            m_errorCode = errorCode;
        }
    }
}

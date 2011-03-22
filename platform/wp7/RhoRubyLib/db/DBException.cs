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
    public class CDBException : Exception
    {
        private int _errorCode;
        public int ErrorCode
        {
            get { return _errorCode; }
            set { _errorCode = value; }
        }

        public CDBException(Exception exc)
            : base("Message: " + exc.Message)
        {
            _errorCode = 0;
	    }

        public CDBException(int errorCode, string message)
            : base(message)
        {
            _errorCode = errorCode;
        }
    }
}

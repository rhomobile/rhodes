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
        private int _errorCode;
        public int ErrorCode
        {
            get { return _errorCode; }
            set { _errorCode = value; }
        }
        public DBException(int errorCode, string message)
            : base(message)
        {
            _errorCode = errorCode;
        }
    }
}

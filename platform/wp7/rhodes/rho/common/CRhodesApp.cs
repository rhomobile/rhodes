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
using Microsoft.Phone.Controls;
using RhoRuby;

namespace Rhodes
{
    public sealed class CRhodesApp
    {
        private static readonly CRhodesApp instance = new CRhodesApp();

        private WebBrowser _webBrowser;
        private RhoRubyFramework _rhoframework;

        public WebBrowser WebBrowser
        {
            get { return _webBrowser; }
            set { _webBrowser = value; }
        }

        public void Init()
        {
            RhoRubyFramework.WebBrowser = _webBrowser;
            _rhoframework = new RhoRubyFramework();
        }

        private CRhodesApp() {}

        public static CRhodesApp Instance
        {
            get
            {
                return instance;
            }
        }
    }
}

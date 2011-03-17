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
using rho.net;
using RhoRuby;

namespace rho.common
{
    public sealed class CRhodesApp
    {
        private static readonly CRhodesApp instance = new CRhodesApp();

        private WebBrowser _webBrowser;
        private RhoRubyFramework _rhoframework;
        private CHttpServer m_httpServer;

        public WebBrowser WebBrowser
        {
            get { return _webBrowser; }
            set { _webBrowser = value; }
        }
        public CHttpServer HttpServer
        {
            get { return m_httpServer; }
        }

        private CRhodesApp() {}

        public static CRhodesApp Instance
        {
            get
            {
                return instance;
            }
        }

        public void Init()
        {
            m_httpServer = new CHttpServer(CFilePath.join(getRhoRootPath(), "apps"));

            RhoRubyFramework.WebBrowser = _webBrowser;
            _rhoframework = new RhoRubyFramework();
        }

        String getRhoRootPath()
        {
            return "/rho/";
        }
    }
}

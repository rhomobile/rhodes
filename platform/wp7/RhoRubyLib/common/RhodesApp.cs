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
using rho;

namespace rho.common
{
    public sealed class CRhodesApp
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("RhodesApp");

        private static readonly CRhodesApp m_instance = new CRhodesApp();
        public static CRhodesApp Instance { get { return m_instance; } }
        private CRhodesApp() { }

        private WebBrowser m_webBrowser;
        private CHttpServer m_httpServer;

        public WebBrowser WebBrowser{ get { return m_webBrowser; } }
        public CHttpServer HttpServer{ get { return m_httpServer; } }
        public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

        public void Init(WebBrowser browser)
        {
            LOG.INFO("Init");

            try
            {
                throw new InvalidOperationException("My OP");
            }
            catch (Exception e)
            {
                LOG.ERROR("Test error.", e);
            }

            m_webBrowser = browser;
            m_httpServer = new CHttpServer(CFilePath.join(getRhoRootPath(), "apps"));
            CRhoResourceMap.deployContent();
            RhoRuby.Init(m_webBrowser);
        }

        public String canonicalizeRhoUrl(String url)
        {
            if (!url.StartsWith(getRhoRootPath()))
                return CFilePath.join(getRhoRootPath(), url);

            return url;
        }

        String getRhoRootPath()
        {
            return "/";
        }
    }
}

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
        int m_currentTabIndex = 0;
        String[] m_currentUrls = new String[5];
        private String m_strBlobsDirPath, m_strDBDirPath;
        //private String m_strStartUrl, m_strHomeUrl;

        public WebBrowser WebBrowser{ get { return m_webBrowser; } }
        public CHttpServer HttpServer{ get { return m_httpServer; } }
        public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

        public String getBlobsDirPath() { return m_strBlobsDirPath; }

        public void Init(WebBrowser browser)
        {
            initAppUrls();
            RhoLogger.InitRhoLog();
            LOG.INFO("Init");

            m_webBrowser = browser;
            m_httpServer = new CHttpServer(CFilePath.join(getRhoRootPath(), "apps"));
            CRhoResourceMap.deployContent();
            RhoRuby.Init(m_webBrowser);
        }

        void initAppUrls()
        {
            //m_strHomeUrl = "http://localhost:2375";
            m_strBlobsDirPath = getRhoRootPath() + "db/db-files";
            m_strDBDirPath = getRhoRootPath() + "db";
        }

        public String canonicalizeRhoUrl(String url)
        {
            if (!url.StartsWith(getRhoRootPath()))
                return CFilePath.join(getRhoRootPath(), url);

            return url;
        }

        public static String getRhoRootPath()
        {
            return "/";
        }

        public String resolveDBFilesPath(String strFilePath)
        {
            if (strFilePath.startsWith(getRhoRootPath()))
                return strFilePath;

            return CFilePath.join(getRhoRootPath(), strFilePath);
        }

        public String getCurrentUrl(int index)
        {
            return m_currentUrls[m_currentTabIndex];
        }

        public void keepLastVisitedUrl(String strUrl)
        {
            m_currentUrls[m_currentTabIndex] = canonicalizeRhoUrl(strUrl);
        }

        public String getPlatform()
        {
            return "WINDOWS_PHONE";
        }

        public bool isRhodesAppUrl(String url)
        {
            //TODO: isRhodesAppUrl
            return false;
        }

        public boolean unzip_file(String path)
        {
            //TODO: unzip_file
            return false;
        }
    }
}

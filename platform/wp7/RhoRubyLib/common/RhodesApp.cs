using System;
using Microsoft.Phone.Controls;
using rho.net;
using rho;
using rho.db;
using rho.sync;
using System.Threading;
using System.Collections.Generic;
using Microsoft.Phone.Shell;
using IronRuby.Builtins;
using System.Windows.Media;
using System.Windows.Navigation;

namespace rho.common
{
    public sealed class CRhodesApp
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("RhodesApp");
        RhoConf RHOCONF() { return RhoConf.getInstance(); }

        private static readonly CRhodesApp m_instance = new CRhodesApp();
        public static CRhodesApp Instance { get { return m_instance; } }
        private CRhodesApp() { }

        private WebBrowser m_webBrowser;
        private PhoneApplicationPage m_appMainPage;
        private Stack<Uri> m_backHistory = new Stack<Uri>();
        private Stack<Uri> m_forwardHistory = new Stack<Uri>();
        private Uri m_currentUri = null; 
        private CHttpServer m_httpServer;
        int m_currentTabIndex = 0;
        String[] m_currentUrls = new String[5];
        private String m_strBlobsDirPath, m_strDBDirPath;
        private String m_strHomeUrl;
        ManualResetEvent m_UIWaitEvent = new ManualResetEvent(false);

        public WebBrowser WebBrowser{ get { return m_webBrowser; } }
        public CHttpServer HttpServer{ get { return m_httpServer; } }
        public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

        public String getBlobsDirPath() { return m_strBlobsDirPath; }
        public String getHomeUrl() { return m_strHomeUrl; }

        public void Init(WebBrowser browser, PhoneApplicationPage appMainPage)
        {
            initAppUrls();
            RhoLogger.InitRhoLog();
            LOG.INFO("Init");

            CRhoFile.recursiveCreateDir(CFilePath.join(getBlobsDirPath()," "));

            m_webBrowser = browser;
            m_appMainPage = appMainPage;
            m_appMainPage.ApplicationBar = null;
            m_httpServer = new CHttpServer(CFilePath.join(getRhoRootPath(), "apps"));
            CRhoResourceMap.deployContent();
            RhoRuby.Init(m_webBrowser);

            DBAdapter.initAttrManager();

            LOG.INFO("Starting sync engine...");
            SyncThread sync = null;
            try{
	        	sync = SyncThread.Create();
	        	
	        }catch(Exception exc){
	        	LOG.ERROR("Create sync failed.", exc);
	        }
	        if (sync != null) {
	        	//sync.setStatusListener(this);
	        }
	        
	        RhoRuby.InitApp();
	        RhoRuby.call_config_conflicts();
            RHOCONF().conflictsResolved();
        }

        public void stopApp()
        {
            string[] ar1 = CRhoFile.enumDirectory("db");

            RhoRuby.Stop();
            SyncThread.getInstance().Destroy();
            RhoLogger.close();
            m_UIWaitEvent.Close();

            string[] ar2 = CRhoFile.enumDirectory("db");
            int i = 0;
            //net::CAsyncHttp::Destroy();
        }

        void initAppUrls()
        {
            m_strHomeUrl = "http://localhost:2375";
            m_strBlobsDirPath = getRhoRootPath() + "db/db-files";
            m_strDBDirPath = getRhoRootPath() + "db";
        }

        boolean isExternalUrl(String strUrl)
        {
            return strUrl.startsWith("http://") || strUrl.startsWith("https://") ||
                strUrl.startsWith("javascript:") || strUrl.startsWith("mailto:")
                 || strUrl.startsWith("tel:") || strUrl.startsWith("wtai:") ||
                 strUrl.startsWith("sms:");
        }

        public String canonicalizeRhoPath(String path)
        {
            return path;
        }

        public String canonicalizeRhoUrl(String url)
        {
            if (url == null || url.length() == 0)
                return getHomeUrl();

            String strUrl = url.Replace('\\', '/');
            if (!strUrl.startsWith(getHomeUrl()) && !isExternalUrl(strUrl))
                strUrl = CFilePath.join(getHomeUrl(), strUrl);

            return strUrl;
        }

        public boolean isRhodesAppUrl(String url)
        {
            return url.startsWith(getHomeUrl());
        }

        private delegate void CallbackDelegate(String strUrl, String strBody);

        private void doProcessCallback(String strUrl, String strBody)
        {
            String strReply;
            m_httpServer.call_ruby_method(strUrl, strBody, out strReply);

            m_UIWaitEvent.Set();
        }

        public NetResponse processCallback(String strUrl, String strBody)
        {
            if (m_webBrowser.Dispatcher.CheckAccess())
                doProcessCallback(strUrl, strBody);
            else
            {
                m_webBrowser.Dispatcher.BeginInvoke(new CallbackDelegate(doProcessCallback), strUrl, strBody);
                m_UIWaitEvent.Reset();
                m_UIWaitEvent.WaitOne();
            }

            return new NetResponse("", 200);
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

        public boolean unzip_file(String path)
        {
            //TODO: unzip_file
            return false;
        }

        private static Color getColorFromString(String strColor)
        {
            if (strColor == null || strColor == "")
                return Color.FromArgb(255, 0, 0, 0);

	        int c = Convert.ToInt32(strColor);

	        int cR = (c & 0xFF0000) >> 16;
	        int cG = (c & 0xFF00) >> 8;
	        int cB = (c & 0xFF);

            return Color.FromArgb(255, Convert.ToByte(cR), Convert.ToByte(cG), Convert.ToByte(cB));
        }

        private String getDefaultImagePath(String strAction)
        {
            String strImagePath = "";
            if ( strAction == "options" )
                strImagePath = "lib/res/options_btn.png";
            else if ( strAction == "home" )
                strImagePath = "lib/res/home_btn.png";
            else if ( strAction == "refresh" )
                strImagePath = "lib/res/refresh_btn.png";
            else if ( strAction == "back" )
                strImagePath = "lib/res/back_btn.png";
            else if ( strAction == "forward" )
                strImagePath = "lib/res/forward_btn.png";

            return strImagePath.Length > 0 ? CFilePath.join(getRhoRootPath(), strImagePath) : null;
        }   

        private void createToolBarButtons(int barType, Object[] hashArray)
        {
            for (int i = 0; hashArray != null && i < hashArray.Length; i++)
            {
                if (i == 5) break;//ApplicationBar Allows developers to create and display an application bar
                                  //with between 1 and 4 buttons and a set of text menu items
                                  //in Windows Phone applications.

                if (hashArray[i] != null && hashArray[i] is Hash)
                {
                    String action = null;
                    String icon = null;
                    String label = null;
                    object val = null;

                    Hash values = (Hash)hashArray[i];
                    if (values.TryGetValue((object)MutableString.Create("action"), out val))
                        action = val.ToString();
                    if (values.TryGetValue((object)MutableString.Create("icon"), out val))
                        icon = val.ToString();
                    if (values.TryGetValue((object)MutableString.Create("label"), out val))
                        label = val.ToString();

                    if (label == null && barType == 0)
                        label = ".";//Text can not be empty. it's WP7's restriction!!!

                    if (icon == null)//icon can not be null or empty. so now i don't know how to create separator
                    {
                        icon = getDefaultImagePath(action);
                    }

                    if (icon == null || action == null)
                        continue;

                    if (action == "forward" && RHOCONF().getBool("jqtouch_mode"))
                        continue;
                    ApplicationBarIconButton button = new ApplicationBarIconButton(new Uri(icon, UriKind.Relative));
                    button.Text = label;
                    button.Click += delegate(object sender, EventArgs e) { processToolBarCommand(sender, e, action); };

                    m_appMainPage.ApplicationBar.Buttons.Add(button);
                }
            }
        }

        public void createToolBar(int barType, Object barParams)
        {          
            m_appMainPage.ApplicationBar = new ApplicationBar();
            m_appMainPage.ApplicationBar.IsMenuEnabled = true;
            m_appMainPage.ApplicationBar.IsVisible = true;
            m_appMainPage.ApplicationBar.Opacity = 1.0;

            Object[] hashArray = null;
            Hash paramHash = null;
            object val = null;

            if (barParams is RubyArray)
                hashArray = ((RubyArray)barParams).ToArray();
            else
                paramHash = (Hash)barParams;

            if (paramHash != null && paramHash.TryGetValue((object)MutableString.Create("background_color"), out val))
                m_appMainPage.ApplicationBar.BackgroundColor = getColorFromString(val.ToString());

            if (paramHash != null && paramHash.TryGetValue((object)MutableString.Create("buttons"), out val) && val is RubyArray)
                hashArray = ((RubyArray)val).ToArray();

            createToolBarButtons(barType, hashArray);
        }

        public void removeToolBar()
        {
            if (m_appMainPage.ApplicationBar != null)
            {
                m_appMainPage.ApplicationBar.MenuItems.Clear();
                m_appMainPage.ApplicationBar.IsMenuEnabled = false;
                m_appMainPage.ApplicationBar.IsVisible = false;
            }
        }

        public void addToHistory(Uri uri)
        {
            Uri previous = null;
            if (m_backHistory.Count > 0)
                previous = m_backHistory.Peek();

            if (uri == previous)
            {
                m_backHistory.Pop();
                m_forwardHistory.Push(m_currentUri);
            }
            else
            {
                if (m_currentUri != null)
                {
                    m_backHistory.Push(m_currentUri);
                    if (m_forwardHistory.Count > 0)
                        m_forwardHistory.Pop();
                }
            }
            m_currentUri = uri; 
        }

        private void processToolBarCommand(object sender, EventArgs e,  String strAction)
        {

            if (strAction == "back" && m_backHistory.Count > 0)
            {
                Uri destination = m_backHistory.Peek();
                m_webBrowser.Navigate(destination);
            }

            if (strAction == "forward" && m_forwardHistory.Count > 0)
            {
                Uri destination = m_forwardHistory.Peek();
                m_webBrowser.Navigate(destination);

            }

            if (strAction == "home")
            {
                String strHomePage = RhoRuby.getStartPage();
    	        String strStartPage = canonicalizeRhoUrl(strHomePage);
                m_webBrowser.Navigate(new Uri(strStartPage));
            }

            if (strAction == "refresh" && m_currentUri != null)
                m_webBrowser.Navigate(m_currentUri);
        }
    }
}

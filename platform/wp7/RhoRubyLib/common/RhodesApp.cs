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
        private Hash m_menuItems = null;
        private Uri m_currentUri = null; 
        private CHttpServer m_httpServer;
        private bool m_barIsStarted = false;
        int m_currentTabIndex = 0;
        String[] m_currentUrls = new String[5];
        private String m_strBlobsDirPath, m_strDBDirPath;
        private String m_strHomeUrl;
        private String m_strAppBackUrl;
        ManualResetEvent m_UIWaitEvent = new ManualResetEvent(false);
        Vector<Object> m_arCallbackObjects = new Vector<Object>();

        public WebBrowser WebBrowser{ get { return m_webBrowser; } }
        public CHttpServer HttpServer{ get { return m_httpServer; } }
        public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        public bool barIsStarted { get { return m_barIsStarted; } }

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
        }

        public void startApp()
        {
            LOG.INFO("startApp");

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

        public void closeApp()
        {
            m_httpServer.stop(10);

            RhoLogger.close();
            m_UIWaitEvent.Close();
        }

        public void stopApp()
        {
            //string[] ar1 = CRhoFile.enumDirectory("db");

            RhoRuby.callUIDestroyed();

            SyncThread.getInstance().Destroy();
            CAsyncHttp.Destroy();

            RhoRuby.Stop();

            //string[] ar2 = CRhoFile.enumDirectory("db");
            //int i = 0;
            //net::CAsyncHttp::Destroy();
        }

        public bool isMainRubyThread()
        {
            return m_httpServer.CurrentThread == Thread.CurrentThread;
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

        public NetResponse processCallback(String strUrl, String strBody)
        {
//            m_webBrowser.Dispatcher.BeginInvoke( () =>
//            {
                String strReply;
                m_httpServer.call_ruby_method(strUrl, strBody, out strReply);

//                m_UIWaitEvent.Set();
//            });

//            m_UIWaitEvent.Reset();
//            m_UIWaitEvent.WaitOne();

            return new NetResponse("", 200);
        }

        public void processWebNavigate(String strUrl)
        {
            m_webBrowser.Dispatcher.BeginInvoke( () =>
            {
                m_webBrowser.IsScriptEnabled = true;
                m_webBrowser.Navigate(new Uri(strUrl, UriKind.Relative));
            });
        }

        public void processInvokeScript(String strScript)
        {
            m_webBrowser.Dispatcher.BeginInvoke(() =>
            {
                m_webBrowser.InvokeScript(strScript);
            });
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
            int bCount = 0;
            for (int i = 0; hashArray != null && i < hashArray.Length; i++)
            {
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

                    if (action == "forward" || action == "close")// && RHOCONF().getBool("jqtouch_mode"))
                        continue;
                    ApplicationBarIconButton button = new ApplicationBarIconButton(new Uri(icon, UriKind.Relative));
                    button.Text = label;
                    button.Click += delegate(object sender, EventArgs e) { processToolBarCommand(sender, e, action); };

                    m_appMainPage.ApplicationBar.Buttons.Add(button);
                    //ApplicationBar Allows developers to create and display an application bar
                    //with between 1 and 4 buttons and a set of text menu items
                    //in Windows Phone applications.
                    if (++bCount == 4)
                        return;
                }
            }
        }

        public void setMenuItems(Hash menuItems)
        {
            m_webBrowser.Dispatcher.BeginInvoke( () =>
            {
                if (m_appMainPage.ApplicationBar == null)
                    createEmptyToolBar();
                else
                {
                    m_appMainPage.ApplicationBar.MenuItems.Clear();
                    setAppBackUrl("");
                }

                m_menuItems = menuItems;

                foreach (KeyValuePair<object, object> kvp in m_menuItems)
                {
                    ApplicationBarMenuItem item = new ApplicationBarMenuItem();
                    item.Text = kvp.Key.ToString();
                    String action = null; 
                    if (kvp.Value == null) 
                        continue;
                    else
                        action = kvp.Value.ToString();

                    if (action == "close") continue;

                    if (item.Text.toLowerCase() == "back" && action.toLowerCase() != "back") setAppBackUrl(action);

                    item.Click += delegate(object sender, EventArgs e) { processToolBarCommand(sender, e, action); };

                    m_appMainPage.ApplicationBar.MenuItems.Add(item);
                }
            });
        }

        private void createEmptyToolBar()
        {
            m_appMainPage.ApplicationBar = new ApplicationBar();
            m_appMainPage.ApplicationBar.IsMenuEnabled = true;
            m_appMainPage.ApplicationBar.IsVisible = true;
            m_appMainPage.ApplicationBar.Opacity = 1.0;
            setAppBackUrl("");
        }

        public void createToolBar(int barType, Object barParams)
        {
            m_webBrowser.Dispatcher.BeginInvoke( () =>
            {
                createEmptyToolBar();

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
                setMenuItems(m_menuItems);
                m_barIsStarted = true;
            });
        }

        public void removeToolBar()
        {
            m_webBrowser.Dispatcher.BeginInvoke( () =>
            {
                if (m_appMainPage.ApplicationBar != null)
                {
                    m_appMainPage.ApplicationBar.MenuItems.Clear();
                    m_appMainPage.ApplicationBar.Buttons.Clear();
                    m_appMainPage.ApplicationBar.IsMenuEnabled = false;
                    m_appMainPage.ApplicationBar.IsVisible = false;

                    m_appMainPage.ApplicationBar = null;
                    m_barIsStarted = false;
                }
            });
        }

        public void showLogScreen()
        {
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

            boolean callback = false;
            if (strAction == "back")
            {
                if (m_strAppBackUrl.length() > 0)
                    processToolBarCommand(this, e, m_strAppBackUrl);
                else if (m_backHistory.Count > 0)
                {
                    Uri destination = m_backHistory.Peek();
                    m_webBrowser.Navigate(destination);
                }
                return;
            }

            if (strAction.startsWith("callback:"))
            {
                strAction = strAction.substring(9);
                callback = true;
            }

            if (strAction == "forward" && m_forwardHistory.Count > 0)
            {
                Uri destination = m_forwardHistory.Peek();
                m_webBrowser.Navigate(destination);
                return;
            }

            if (strAction == "home")
            {
                String strHomePage = RhoRuby.getStartPage();
                strHomePage = canonicalizeRhoUrl(strHomePage);
                m_webBrowser.Navigate(new Uri(strHomePage));
                return;
            }

            if (strAction == "log")
            {
                showLogScreen();
                return;
            }

            if (strAction == "options")
            {
                String curUrl = RhoRuby.getOptionsPage();
				curUrl = canonicalizeRhoUrl(curUrl);
			    m_webBrowser.Navigate(new Uri(curUrl));
                return;
            }

            if (strAction == "refresh" && m_currentUri != null)
            {
                m_webBrowser.Navigate(m_currentUri);
                return;
            }

            if (strAction == "sync")
            {
                SyncThread.doSyncAllSources(true);
                return;
            }

            strAction = canonicalizeRhoUrl(strAction);
            if (callback)
            {
                RhoClassFactory.createNetRequest().pushData(strAction, "rho_callback=1", null);
            }
            else
                m_webBrowser.Navigate(new Uri(strAction));
        }

        public String addCallbackObject(Object valObject, String strName)
        {
            int nIndex = -1;
            for (int i = 0; i < (int)m_arCallbackObjects.size(); i++)
            {
                if (m_arCallbackObjects.elementAt(i) == null)
                    nIndex = i;
            }
            if (nIndex == -1)
            {
                m_arCallbackObjects.addElement(valObject);
                nIndex = m_arCallbackObjects.size() - 1;
            }
            else
                m_arCallbackObjects[nIndex] = valObject;

            String strRes = "__rho_object[" + strName + "]=" + nIndex;

            return strRes;
        }

        public Object getCallbackObject(int nIndex)
        {
            if (nIndex < 0 || nIndex > m_arCallbackObjects.size())
                return null;

            Object res = (Object)m_arCallbackObjects.elementAt(nIndex);
            m_arCallbackObjects[nIndex] = null;
            return res;
        }

        public void setAppBackUrl(String url)
        {
            m_strAppBackUrl = url;
            m_backHistory.Clear();
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using System.Windows.Navigation;
using IronRuby.Builtins;
using rho.common;

namespace rho.views
{
    public partial class RhoView : UserControl
    {
        private Stack<Uri> m_backHistory = new Stack<Uri>();
        private Stack<Uri> m_forwardHistory = new Stack<Uri>();
        private Hash m_menuItems = null;
        private Uri m_currentUri = null;
        private PhoneApplicationPage m_mainPage = null;
        private Grid m_layoutRoot = null;
        public  PhoneApplicationPage MainPage { set { m_mainPage = value; } }
        public  Grid MainPageLayoutRoot { set { m_layoutRoot = value; } }
        private String m_strAction = null;
        private bool m_reload = false;
        private bool m_loadFirstTime = true;
        private bool m_masterView = false;
        //TO DO history. each time we have to save our current state of rhodesapp in rhoview entity
        public Stack<Uri> BackHistory { set { m_backHistory = value; } }
        public Stack<Uri> ForwardHistory { set { m_forwardHistory = value; } }
        public bool MasterView { get { return m_masterView; } set { m_masterView = value; } }
        public Uri CurrentUri { set { m_currentUri = value; } }

        public RhoView()
        {
            InitializeComponent();
            webBrowser1.IsScriptEnabled = true;

            webBrowser1.Loaded += WebBrowser_OnLoaded;
            //webBrowser1.LoadCompleted += WebBrowser_OnLoadCompleted;
            webBrowser1.Navigating += WebBrowser_OnNavigating;
            webBrowser1.Navigated += WebBrowser_OnNavigated;
        }
        
        public RhoView(PhoneApplicationPage mainPage, Grid layoutRoot, String strAction,
                       bool reload, Brush webBkgColor)
        {
            InitializeComponent();
            webBrowser1.IsScriptEnabled = true;

            webBrowser1.Loaded += WebBrowser_OnLoaded;
            //webBrowser1.LoadCompleted += WebBrowser_OnLoadCompleted;
            webBrowser1.Navigating += WebBrowser_OnNavigating;
            webBrowser1.Navigated += WebBrowser_OnNavigated;
            //webBrowser1.ScriptNotify += WebBrowser_OnScriptNotify;
            m_mainPage = mainPage;
            m_strAction = strAction;
            m_layoutRoot = layoutRoot;
            m_reload = reload;
            if (webBkgColor != null)
                webBrowser1.Background = webBkgColor;
        }

        private CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }

        private void WebBrowser_OnLoaded(object sender, RoutedEventArgs e)
        {
            if (RHODESAPP().rhoView != null)
            {
                RHODESAPP().rhoView.BackHistory = RHODESAPP().BackHistory;
                RHODESAPP().rhoView.ForwardHistory = RHODESAPP().ForwardHistory;
                RHODESAPP().rhoView.CurrentUri = RHODESAPP().CurrentUri;
            }
            RHODESAPP().Init(webBrowser1, m_mainPage, m_layoutRoot, this);
            RHODESAPP().BackHistory = m_backHistory;
            RHODESAPP().ForwardHistory = m_forwardHistory;
            RHODESAPP().CurrentUri = m_currentUri;
            if ( ((!m_loadFirstTime && m_reload) || m_loadFirstTime) && m_strAction != null)
            {
                if (m_loadFirstTime)
                    m_loadFirstTime = false;
                bool callback = false;
                if (m_strAction.startsWith("callback:"))
                {
                    m_strAction = m_strAction.substring(9);
                    callback = true;
                }
                m_strAction = RHODESAPP().canonicalizeRhoUrl(m_strAction);
                if (callback)
                {
                    RhoClassFactory.createNetRequest().pushData(m_strAction, "rho_callback=1", null);
                }
                else
                    webBrowser1.Navigate(new Uri(m_strAction));
            }
        }

        private void WebBrowser_OnNavigating(object sender, NavigatingEventArgs e)
        {
            if (!RHODESAPP().HttpServer.processBrowserRequest(e.Uri))
                return;

            e.Cancel = true;
        }

        private void WebBrowser_OnNavigated(object sender, NavigationEventArgs e)
        {
            RHODESAPP().addToHistory(e.Uri);
        }

        public void refresh()
        {
            WebBrowser_OnLoaded(this, null);
        }
    }
}

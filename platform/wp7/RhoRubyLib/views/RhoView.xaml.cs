/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
using System.IO;

namespace rho.views
{
    public partial class RhoView : UserControl
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
                    new RhoLogger("RhoView");

        private static RhoLogger JSLOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
                    new RhoLogger("JavaScript");

        private Stack<Uri> m_backHistory = new Stack<Uri>();
        private Stack<Uri> m_forwardHistory = new Stack<Uri>();
        /// <summary>
        /// 
        /// </summary>
        //private Hash m_menuItems = null;
        private Uri m_currentUri = null;
        private PhoneApplicationPage m_mainPage = null;
        private Grid m_layoutRoot = null;
        public  PhoneApplicationPage MainPage { set { m_mainPage = value; } }
        public  Grid MainPageLayoutRoot { set { m_layoutRoot = value; } }
        private String m_strAction = null;
        private bool m_reload = false;
        private bool m_loadFirstTime = true;
        private bool m_masterView = false;
        
        private const string AJAX_CONTEXT_PARAM = "_rho_callbackId";
        private const string JS_NOTIFY_CONSOLE_LOG = "console.log:";
        private const string JS_NOTIFY_CONSOLE_INFO = "console.info:";
        private const string JS_NOTIFY_CONSOLE_WARNING = "console.warn:";
        private const string JS_NOTIFY_CONSOLE_ERROR = "console.error:";
        private const string JS_NOTIFY_REQUEST = "request:";
        private const string REQUEST_URL_SCHEME = "x-wmapp1:";

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
            webBrowser1.ScriptNotify += WebBrowser_OnScriptNotify;
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
            webBrowser1.ScriptNotify += WebBrowser_OnScriptNotify;
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
            OperatingSystem os = Environment.OSVersion;
            Version vs = os.Version;
            if (vs.Minor < 10) m_reload = true;
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
            if (!RHODESAPP().HttpServer.processBrowserRequest(e.Uri, null))
                return;

            e.Cancel = true;
        }

        private void WebBrowser_OnNavigated(object sender, NavigationEventArgs e)
        {
            RHODESAPP().addToHistory(e.Uri);
        }

        private string extractAjaxContext(string uri)
        {
            int idx = uri.indexOf(AJAX_CONTEXT_PARAM) + AJAX_CONTEXT_PARAM.length();
            if (-1 == idx)
                return null;

            string context = uri.substring(idx + 1);  // +1 due to the '=' sign after param name

            idx = context.indexOf('&');
            if (-1 < idx)
                context = context.substring(0, idx);

            return context;
        }

        private void WebBrowser_OnScriptNotify(object sender, NotifyEventArgs e)
        {
            string request = e.Value;

            if (request.startsWith(JS_NOTIFY_CONSOLE_LOG))
            {
                JSLOG.INFO(request.substring(JS_NOTIFY_CONSOLE_LOG.length()).trim());
            }
            else if (request.startsWith(JS_NOTIFY_CONSOLE_INFO))
            {
                JSLOG.INFO(request.substring(JS_NOTIFY_CONSOLE_INFO.length()).trim());
            }
            else if (request.startsWith(JS_NOTIFY_CONSOLE_WARNING))
            {
                JSLOG.WARNING(request.substring(JS_NOTIFY_CONSOLE_WARNING.length()).trim());
            }
            else if (request.startsWith(JS_NOTIFY_CONSOLE_ERROR))
            {
                JSLOG.ERROR(request.substring(JS_NOTIFY_CONSOLE_ERROR.length()).trim());
            }
            else if (request.startsWith(JS_NOTIFY_REQUEST))
            {
                string req = request.substring(JS_NOTIFY_REQUEST.length()).trim();

                string ajaxContext = extractAjaxContext(req);
                if (!RHODESAPP().HttpServer.processBrowserRequest(new Uri(req.substring(REQUEST_URL_SCHEME.length()), UriKind.Relative), ajaxContext))
                {
                    LOG.ERROR("External requests should be filtered in javascript");
                }
            }
        }

        public void refresh()
        {
            WebBrowser_OnLoaded(this, null);
        }
    }
}

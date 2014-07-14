using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using rhoruntime;

namespace rhodes
{
    public sealed partial class MainPage : Page
    {
        // saved singletone instance of MainPage
        static private MainPage _instance = null;
        // saved id of the UI thread
        private int _uiThreadID = -1;
        // rhodes main thread
        private Thread _rhoruntimeThread;
        private bool _isCallbackFired = false;
        // internal variables
        private double _screenWidth;
        private double _screenHeight;
        private double _screenPhysicalWidth;
        private double _screenPhysicalHeight;
        private bool _isBrowserInitialized = false;
        private int _tabIndex = -1;
        // HTML code to get true User-Agent string
        private const string getUserAgentHtml =
        @"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 4.01 Transitional//EN"">
        <html>
        <head>
        <script language=""javascript"" type=""text/javascript"">
            function notifyUA() {
               window.external.notify(navigator.userAgent);
            }
        </script>
        </head>
        <body onload=""notifyUA();""></body>
        </html>";
        // web browser user agent string (initialized with fabricated default value)
        private string _userAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows Phone " +
            System.Environment.OSVersion.Version.Major.ToString() + "." +
            System.Environment.OSVersion.Version.Minor.ToString() +
            "; Trident/6.0; IEMobile/10.0; ARM; Touch; " +
            Microsoft.Phone.Info.DeviceStatus.DeviceManufacturer + "; " +
            Microsoft.Phone.Info.DeviceStatus.DeviceName + ")";

        private string initUri = "";
        private PageOrientation _screenOrientation = PageOrientation.None;

        public bool isBrowserInitialized(int index)
        {
            return _isBrowserInitialized; // (index == -1) || !_tabProps.ContainsKey(index) ? _isBrowserInitialized : _tabProps[index]._isInitialized;
        }

        private bool isUIThread
        {
            get { return _uiThreadID == System.Threading.Thread.CurrentThread.ManagedThreadId; }
        }

        static public MainPage getInstance()
        {
            return _instance;
        }

        public bool isEmulator()
        {
            return DeviceStatus.DeviceName.Contains("Emulator") == true ? true : false;
        }

        public MainPage()
        {
            _instance = this;
            _uiThreadID = System.Threading.Thread.CurrentThread.ManagedThreadId;
            updateScreenSize();
            this.InitializeComponent();
            ApplicationBar.IsVisible = false;
            _screenOrientation = Orientation;
            try
            {
                // initialize C# extensions factories
                CSharpExtensions.InitializeExtensions();
                // create rhodes runtime object
                var _rhoruntime = CRhoRuntime.getInstance(new MainPageWrapper(this));
                _rhoruntime.setCryptoEngine(new CryptoEngineWrapper(new RhoCrypt()));
                // create and start rhodes main thread               
                _rhoruntimeThread = new Thread(_rhoruntime.Execute);
                _rhoruntimeThread.Start();

                //temporary solutions, to do refactoring
                //Thread.Sleep(200);
                _rhoruntime.onActivate(0);
            }
            catch (Exception e)
            {
                RhodesWebBrowser.NavigateToString("<html><head><title>Exception</title></head><body>Exception: " + e.Message + "</body></html>");
            }
        }

        private void RhodesWebBrowser_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            //
        }

        private void RhodesWebBrowser_NavigationFailed(object sender, WebViewNavigationFailedEventArgs e)
        {
            //
        }

        private void RhodesWebBrowser_LoadCompleted(object sender, NavigationEventArgs e)
        {
            //
        }

        private void RhodesWebBrowser_Loaded(object sender, RoutedEventArgs e)
        {
            //
        }

        private void RhodesWebBrowser_Unloaded(object sender, RoutedEventArgs e)
        {
            //
        }

        private void RhodesWebBrowser_ScriptNotify(object sender, NotifyEventArgs e)
        {
            String answer = CRhoRuntime.getInstance().onJSInvoke(e.Value);
            WebBrowser wb = sender as WebBrowser;
            wb.IsScriptEnabled = true;
            wb.InvokeScript("__rhoNativeApiResult", new string[] { answer });
        }

        private void Page_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            updateScreenSize();
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            var browser = new Microsoft.Phone.Controls.WebBrowser();
            browser.IsScriptEnabled = true;
            browser.Visibility = Visibility.Collapsed;
            browser.Loaded += (asender, args) => browser.NavigateToString(getUserAgentHtml);
            browser.ScriptNotify += (asender, args) =>
            {
                string userAgent = args.Value;
                LayoutRoot.Children.Remove(browser);
                this._userAgent = userAgent;
            };
            LayoutRoot.Children.Add(browser);
        }

        internal int getLogicalDpiX()
        {
            return (int)(_screenWidth / _screenPhysicalWidth);
        }

        internal int getLogicalDpiY()
        {
            return (int)(_screenHeight / _screenPhysicalHeight);
        }

        internal int getScreenWidth()
        {
            // TODO: consider rotation
            return (int)_screenWidth;
        }

        internal int getScreenHeight()
            {
            // TODO: consider rotation
            return (int)_screenHeight;
        }

        internal string getUserAgent()
        {
            return this._userAgent;
        }

        internal string getWebviewFramework()
        {
            return "IE/" + System.Environment.OSVersion.Version.Major.ToString() + "." +
                System.Environment.OSVersion.Version.Minor.ToString();
        }

        internal string getScreenOrientation()
        {
            if (_screenOrientation == PageOrientation.Portrait ||
                _screenOrientation == PageOrientation.PortraitDown ||
                _screenOrientation == PageOrientation.PortraitUp ||
                _screenOrientation == PageOrientation.None)
            {
                return "portrait";
            }
            else
            //if (_screenOrientation == PageOrientation.Landscape ||
            //    _screenOrientation == PageOrientation.LandscapeLeft ||
            //    _screenOrientation == PageOrientation.LandscapeRight)
            {
                return "landscape";
            }
        }

        internal void bringToFront()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { bringToFront(); }); return; }
            this.bringToFront();
        }

        internal void performOnUiThread(long native)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { performOnUiThread(native); }); return; }
            CRhoRuntime.getInstance().executeRhoRunnable(native);
        }

        private void updateOrientation(PageOrientation orientation)
        {
            _screenOrientation = orientation;
            updateScreenSize();
        }

        private void updateScreenSize()
        {
            _screenWidth = Application.Current.Host.Content.ActualWidth;
            _screenHeight = Application.Current.Host.Content.ActualHeight;
            _screenPhysicalHeight = 4; // assuming 4 inches
            _screenPhysicalWidth = (_screenPhysicalHeight / _screenHeight) * _screenWidth; // assuming square pixels

            if ((_screenOrientation == PageOrientation.Landscape) || (_screenOrientation == PageOrientation.LandscapeLeft) || (_screenOrientation == PageOrientation.LandscapeRight))
            {
                double w = _screenWidth;
                _screenWidth = _screenHeight;
                _screenHeight = w;
                w = _screenPhysicalWidth;
                _screenPhysicalWidth = _screenPhysicalHeight;
                _screenPhysicalHeight = w;
            }
        }

        private string prependWithSlash(string url)
        {
            return url.StartsWith("/") ? url : "/" + url;
        }

        // *** WEBVIEW ***

        internal void navigate(string url, int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigate(url, index); }); return; }
            RhodesWebBrowser.Navigate(new Uri(url, UriKind.RelativeOrAbsolute));
        }

        public string executeScriptFunc(string script, int index)
        {
            string[] codeString = { script };
            _isCallbackFired = false;
            return RhodesWebBrowser.InvokeScript("eval", codeString).ToString();
        }

        internal string executeScript(string script, int index)
        {
            return StringValueByStringIntReturnAgent(executeScriptFunc, script, index);
        }

        internal string executeScriptAsync(string script, int index)
        {
            return StringValueByStringIntReturnAgentAsync(executeScriptFunc, script, index);
        }

        internal void GoBack()
        {
            throw new NotImplementedException();
        }

        internal void GoBack(int index)
        {
            throw new NotImplementedException();
        }

        internal void GoForward()
        {
            throw new NotImplementedException();
        }

        internal void Refresh(int index)
        {
            throw new NotImplementedException();
        }

        internal bool isStarted()
        {
            throw new NotImplementedException();
        }

        internal bool isEmulator()
        {
            throw new NotImplementedException();
        }

        internal string getCurrentURL(int index)
        {
            throw new NotImplementedException();
        }

        internal void toolbarRemoveAllButtons()
        {
            throw new NotImplementedException();
        }

        internal void toolbarShow()
        {
            throw new NotImplementedException();
        }

        internal void toolbarHide()
        {
            throw new NotImplementedException();
        }

        internal int toolbarIsStarted()
        {
            throw new NotImplementedException();
        }

        internal int toolbarGetHeight()
        {
            throw new NotImplementedException();
        }

        internal void toolbarAddAction(string text)
        {
            throw new NotImplementedException();
        }

        internal void toolbarAddAction(string icon, string text, string action)
        {
            throw new NotImplementedException();
        }

        internal void toolbarAddSeparator()
        {
            throw new NotImplementedException();
        }

        internal void setToolbarStyle(bool border, string background, string mask)
        {
            throw new NotImplementedException();
        }

        internal void menuClear()
        {
            throw new NotImplementedException();
        }

        internal void menuAddAction(string text, int item)
        {
            throw new NotImplementedException();
        }

        internal void menuAddSeparator()
        {
            throw new NotImplementedException();
        }

        internal void tabbarInitialize()
        {
            throw new NotImplementedException();
        }

        internal void tabbarRemoveAllTabs()
        {
            throw new NotImplementedException();
        }

        internal void tabbarShow()
        {
            throw new NotImplementedException();
        }

        internal void tabbarHide()
        {
            throw new NotImplementedException();
        }

        internal int tabbarIsStarted()
        {
            throw new NotImplementedException();
        }

        internal int tabbarGetHeight()
        {
            throw new NotImplementedException();
        }

        internal void tabbarRemove(int index)
        {
            throw new NotImplementedException();
        }

        internal void tabbarSwitch(int index)
        {
            throw new NotImplementedException();
        }

        internal int tabbarGetCurrent()
        {
            throw new NotImplementedException();
        }

        internal void tabbarAddTab(string label, string icon, string action, bool disabled, string web_bkg_color, string selected_color, string background_color, bool reload, bool use_current_view_for_tab, bool hasCallback, rhoruntime.IMethodResult oResult)
        {
            throw new NotImplementedException();
        }

        internal void tabbarSetBadge(int index, string badge)
        {
            throw new NotImplementedException();
        }

        internal void exitCommand()
        {
            throw new NotImplementedException();
        }

        internal void navigateBackCommand()
        {
            throw new NotImplementedException();
        }

        internal void navigateForwardCommand()
        {
            throw new NotImplementedException();
        }

        internal void logCommand()
        {
            throw new NotImplementedException();
        }

        internal void refreshCommand(int tab_index)
        {
            throw new NotImplementedException();
        }

        internal void takePicture(string callbackUrl)
        {
            throw new NotImplementedException();
        }

        internal void selectPicture(string callbackUrl)
        {
            throw new NotImplementedException();
        }

        internal void alertHidePopup()
        {
            throw new NotImplementedException();
        }

        internal void fullscreenCommand(int fullScreen)
        {
            throw new NotImplementedException();
        }

        internal void setCookie(string url, string cookie)
        {
            throw new NotImplementedException();
        }

        internal void DoWait(int timeout)
        {
            throw new NotImplementedException();
        }

        internal void DisplayMessage(string msg)
        {
            throw new NotImplementedException();
        }

        // *** MISC ***
        // this method is used as a callback for calling the C# API methods from C++
        public void DoWait(int timeout)
        {
            Thread.Sleep(timeout);
        }

        public void DisplayMessage(string msg)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { DisplayMessage(msg); }); return; }
            RhodesWebBrowser.NavigateToString("<html><head><title>Message</title></head><body>" + msg + "</body></html>");
        }

        private int IntValueReturnAgent(Func<int> func)
        {
            if (isUIThread)
                return func();

            Exception exception = null;
            var waitEvent = new System.Threading.ManualResetEvent(false);
            int return_value = 0;
            Dispatcher.BeginInvoke(() =>
            {
                try
                {
                    return_value = func();
                }
                catch (Exception ex)
                {
                    exception = ex;
                }
                waitEvent.Set();
            });

            waitEvent.WaitOne();
            if (exception != null)
                throw exception;

            return return_value;
        }

        private string StringValueByIntReturnAgent(Func<int, string> func, int index)
        {
            if (isUIThread)
                return func(index);

            Exception exception = null;
            var waitEvent = new System.Threading.ManualResetEvent(false);
            string return_value = "";
            Dispatcher.BeginInvoke(() =>
            {
                try
                {
                    return_value = func(index);
                }
                catch (Exception ex)
                {
                    exception = ex;
                }
                waitEvent.Set();
            });

            waitEvent.WaitOne();
            if (exception != null)
                throw exception;

            return return_value;
        }

        private string StringValueByStringIntReturnAgent(Func<string, int, string> func, string str, int index)
        {
            if (isUIThread)
                return func(str, index);

            Exception exception = null;
            var waitEvent = new System.Threading.ManualResetEvent(false);
            string return_value = "";
            Dispatcher.BeginInvoke(() =>
            {
                try
                {
                    return_value = func(str, index);
                }
                catch (Exception ex)
                {
                    exception = ex;
                }
                waitEvent.Set();
            });

            waitEvent.WaitOne();
            if (exception != null)
                throw exception;

            return return_value;
        }

        private string StringValueByStringIntReturnAgentAsync(Func<string, int, string> func, string str, int index)
        {
            if (isUIThread)
                return func(str, index);

            Exception exception = null;
            string return_value = "";
            Dispatcher.BeginInvoke(() =>
            {
                try
                {
                    return_value = func(str, index);
                }
                catch (Exception ex)
                {
                    exception = ex;
                }
            });

            if (exception != null)
                throw exception;

            return return_value;
        }
    }
}

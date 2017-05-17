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
using Microsoft.Phone.Info;
using System.Threading;
using System.Windows;
using Microsoft.Devices;
using Microsoft.Phone.Controls;
using rhodes.Resources;
using System.IO;
using rhoruntime;
using Microsoft.Phone.Shell;
using System.Windows.Media;
using System.Windows.Controls;
using System.IO.IsolatedStorage;
using rhodes.common;
using System.ComponentModel;
using System.Windows.Media.Imaging;

namespace rhodes
{
    public partial class TabProps
    {
        public bool _isInitialized = false;
        public string _action = null;
        public bool _isLoaded = false;
        public bool _isReload = false;
    };

    public partial class TabHeader : INotifyPropertyChanged
    {
        // internal settings
        public Brush SelectedBackground = null;
        public Brush UnselectedBackground = null;

        // INotifyPropertyChanged impl
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        // properties:

        public string Label { get; set; }
        public ImageSource IconImage { get; set; }

        private Brush _background;
        public Brush Background
        {
            get { return _background; }
            set
            {
                if (_background != value)
                {
                    _background = value;
                    OnPropertyChanged("Background");
                }
            }
        }
    };

    public partial class MainPage : PhoneApplicationPage
    {
        // saved singletone instance of MainPage
        static private MainPage _instance = null;
        private IMethodResult _oTabResult = null;
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

        private Dictionary<int, TabProps> _tabProps = new Dictionary<int, TabProps>();

        private string initUri = "";
        private PageOrientation _screenOrientation = PageOrientation.None;
        // menu items hash table
        private Dictionary<string, int> menuItems = new Dictionary<string, int>();
        // toolbar items hash table
        private Dictionary<string, string> toolbarItems = new Dictionary<string, string>();
        private List<ApplicationBarMenuItem> toolbarMenuItems = new List<ApplicationBarMenuItem>();

        public bool isBrowserInitialized(int index)
        {
            return (index == -1) || !_tabProps.ContainsKey(index) ? _isBrowserInitialized : _tabProps[index]._isInitialized;
        }

        private bool isUIThread
        {
            get { return _uiThreadID == System.Threading.Thread.CurrentThread.ManagedThreadId; }
        }


        private void raiseTabEvent(string eventName, int nOldTab, int nNewTab)
        {
            if (_oTabResult != null)
            {
                Dictionary<string, string> mapRes = new Dictionary<string, string>();
                mapRes["tab_index"] = Convert.ToString(nNewTab);
                mapRes["newTabIndex"] = Convert.ToString(nNewTab);
                mapRes["oldTabIndex"] = Convert.ToString(nOldTab);
                mapRes["tabEvent"] = eventName;
                _isCallbackFired = true;
                _oTabResult.set(mapRes);
            }
        }

        static public MainPage getInstance()
        {
            return _instance;
        }
        static public Grid LayoutGrid()
        {
            return _instance.LayoutRoot;
        }
        public UIElement RootVisual()
        {
            return App.Current.RootVisual;
        }
        public bool isEmulator()
        {
            return DeviceStatus.DeviceName.Contains("Emulator") == true ? true : false;
        }
        public void ApplicationBarEnable(bool Visible)
        {
            ApplicationBar.IsVisible = Visible;
        }
        public bool ApplicationBarStatus()
        {
            return ApplicationBar.IsVisible;
        }

        public MainPage()
        {
            System.Diagnostics.Debug.WriteLine("UserAgent: " + getUserAgent());
            _instance = this;
            _uiThreadID = System.Threading.Thread.CurrentThread.ManagedThreadId;
            updateScreenSize();
            InitializeComponent();
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

        public int getLogicalDpiX()
        {
            return (int)(_screenWidth / _screenPhysicalWidth);
        }

        public int getLogicalDpiY()
        {
            return (int)(_screenHeight / _screenPhysicalHeight);
        }

        public int getScreenWidth()
        {
            // TODO: consider rotation
            return (int)_screenWidth;
        }

        public int getScreenHeight()
        {
            // TODO: consider rotation
            return (int)_screenHeight;
        }

        public string getUserAgent()
        {
            return this._userAgent;
        }

        public string getWebviewFramework()
        {
            return "IE/" + System.Environment.OSVersion.Version.Major.ToString() + "." +
                System.Environment.OSVersion.Version.Minor.ToString();
        }

        public string getScreenOrientation()
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

        public void bringToFront()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { bringToFront(); }); return; }
            this.bringToFront();
        }

        public void performOnUiThread(Int64 native)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { performOnUiThread(native); }); return; }
            CRhoRuntime.getInstance().executeRhoRunnable(native);
        }

        private void PhoneApplicationPage_BackKeyPress(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //e.Cancel = CRhoRuntime.getInstance().onBackKeyPress();
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

        private void PhoneApplicationPage_OrientationChanged(object sender, OrientationChangedEventArgs e)
        {
            updateOrientation(e.Orientation);
        }

        private void PhoneApplicationPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            updateScreenSize();
        }

        private void PhoneApplicationPage_Loaded(object sender, RoutedEventArgs e)
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

        private string prependWithSlash(string url)
        {
            return url.StartsWith("/") ? url : "/" + url;
        }

        // *** WEBVIEW ***

        public void navigate(string url, int index)
        {

            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigate(url, index); }); return; }

            /*if(url.Contains("http://") == false)
            {
                String appdir = Windows.Storage.ApplicationData.Current.LocalFolder.Path;
                url = "file:///" + appdir.Replace(@"\", "/") +"/" + url;
               
            }*/

            if (_tabProps.Count == 0) index = -1;

            if (url == "") return;

            if (index == -1 && !_isBrowserInitialized)
            {
                initUri = url;
                return;
            }
            else if (index > -1 && _tabProps.ContainsKey(index) && _tabProps[index]._isInitialized == false)
            {
                _tabProps[index]._action = url;
                return;
            }

            if (TabbarPivot.Items.Count == 0)
                RhodesWebBrowser.Navigate(new Uri(url, UriKind.RelativeOrAbsolute));
            else
                ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).Navigate(new Uri(url, UriKind.RelativeOrAbsolute));
        }

        public string executeScriptFunc(string script, int index)
        {
            string[] codeString = { script };
            if (TabbarPivot.Items.Count == 0 || _isCallbackFired)//_tabProps[index]._isInitialized == false)
            {
                _isCallbackFired = false;
                return RhodesWebBrowser.InvokeScript("eval", codeString).ToString();
            }
            else
                return ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).InvokeScript("eval", codeString).ToString();
        }

        public string executeScript(string script, int index)
        {
            return StringValueByStringIntReturnAgent(executeScriptFunc, script, index);
        }

        public string executeScriptAsync(string script, int index)
        {
            return StringValueByStringIntReturnAgentAsync(executeScriptFunc, script, index);
        }

        public void GoBack()
        {
            GoBack(TabbarPivot.SelectedIndex);
        }

        public void GoBack(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { GoBack(index); }); return; }
            if ((TabbarPivot.Items.Count == 0)/* || (index < 0) || (index >= TabbarPivot.Items.Count)*/)
            {
                if (RhodesWebBrowser.CanGoBack)
                    RhodesWebBrowser.GoBack();
            }
            else
            {
                if (((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).CanGoBack)
                    ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).GoBack();
            }
        }

        public void GoForward()
        {
            GoForward(TabbarPivot.SelectedIndex);
        }

        public void GoForward(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { GoForward(index); }); return; }
            if ((TabbarPivot.Items.Count == 0) || (index < 0) || (index >= TabbarPivot.Items.Count))
            {
                if (RhodesWebBrowser.CanGoForward)
                    RhodesWebBrowser.GoForward();
            }
            else
            {
                if (((WebBrowser)((PivotItem)TabbarPivot.Items[index]).Content).CanGoForward)
                    ((WebBrowser)((PivotItem)TabbarPivot.Items[index]).Content).GoForward();
            }
        }

        public void Refresh(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { Refresh(index); }); return; }
            if (TabbarPivot.Items.Count == 0)
                RhodesWebBrowser.Navigate(new Uri(RhodesWebBrowser.Source.AbsoluteUri, UriKind.Absolute));
            // another possible implementation: RhodesWebBrowser.InvokeScript("eval", "history.go()");
            else
                ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).Navigate(
                    new Uri(((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).Source.AbsoluteUri, UriKind.Absolute));
        }

        public bool isStarted()
        {
            // TODO: implement WebView->isStarted
            return true;
        }

        private int getValidTabbarIndex(int index)
        {
            return ((index < 0) || (index >= TabbarPivot.Items.Count)) && (TabbarPivot.SelectedIndex >= 0) && (TabbarPivot.SelectedIndex < TabbarPivot.Items.Count) ? TabbarPivot.SelectedIndex : index;
        }

        private string getCurrentURLFunc(int index)
        {
            try
            {
                if ((TabbarPivot.Items.Count == 0) || (index < 0) || (index >= TabbarPivot.Items.Count))
                    return RhodesWebBrowser.Source.ToString();
                else
                    return ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).Source.ToString();
            }
            catch (Exception)
            {
                return "";
            }
        }

        public string getCurrentURL()
        {
            return StringValueByIntReturnAgent(getCurrentURLFunc, -1);
        }

        public string getCurrentURL(int index)
        {
            return StringValueByIntReturnAgent(getCurrentURLFunc, index);
        }

        private void RhodesWebBrowser_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            String url = getCurrentURLFunc((sender as WebBrowser).TabIndex);
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
        }

        private void RhodesWebBrowser_NavigationFailed(object sender, System.Windows.Navigation.NavigationFailedEventArgs e)
        {
            // TODO: WebView NavigationFailed - do we need this?
        }

        private void RhodesWebBrowser_LoadCompleted(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            String url = getCurrentURLFunc((sender as WebBrowser).TabIndex);
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
            int index = (sender as WebBrowser).TabIndex;
            if (index > -1 && _tabProps.ContainsKey(index) && !_tabProps[index]._isInitialized)
            {
                _tabProps[index]._isInitialized = true;
                if (_tabProps[index]._action != null)
                    navigate(_tabProps[index]._action, index);
            }
            else if (TabbarPivot.Items.Count == 0 && !_isBrowserInitialized)
            {
                _isBrowserInitialized = true;
                navigate(initUri, -1);
            }
            else
            {
                if (TabbarPivot.Items.Count > 0 && index > -1 && _tabProps.ContainsKey(index) && url.Contains("about:blank") == false)
                {
                    if (_tabProps[index]._isLoaded == false)
                        _tabProps[index]._isLoaded = true;
                }
            }
        }

        private void RhodesWebBrowser_Loaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc((sender as WebBrowser).TabIndex));
        }


        private void RhodesWebBrowser_Unloaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc((sender as WebBrowser).TabIndex));
        }

        private void RhodesWebBrowser_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            // TODO: webview size changed event handler ?
        }

        private void updateAppBarModeAndVisibility(bool autoToolbarVisibility = true)
        {
            if (autoToolbarVisibility && (ApplicationBar.Buttons.Count > 0))
            {
                ApplicationBar.Mode = ApplicationBarMode.Default;
                ApplicationBar.IsVisible = true;
                ApplicationBar.IsMenuEnabled = ApplicationBar.MenuItems.Count > 0;
                ApplicationBar.Opacity = 1.0;
            }
            else if (ApplicationBar.MenuItems.Count > 0)
            {
                ApplicationBar.Mode = ApplicationBarMode.Minimized;
                ApplicationBar.IsVisible = true;
                ApplicationBar.IsMenuEnabled = true;
                ApplicationBar.Opacity = 0.75;
            }
            else
            {
                ApplicationBar.Mode = ApplicationBarMode.Default;
                ApplicationBar.IsVisible = false;
                ApplicationBar.IsMenuEnabled = false;
                ApplicationBar.Opacity = 1.0;
            }
        }

        // *** TOOLBAR ***

        public void toolbarRemoveAllButtons()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarRemoveAllButtons(); }); return; }
            ApplicationBar.Buttons.Clear();
            toolbarItems.Clear();
            // remove all toolbar items added to menu (wp8 toolbar allows 4 buttons only)
            int j = toolbarMenuItems.Count - 1;
            if (j >= 0)
            {
                for (int i = ApplicationBar.MenuItems.Count - 1; i >= 0; i--)
                {
                    if (ApplicationBar.MenuItems[i].Equals(toolbarMenuItems[j]))
                    {
                        ApplicationBar.MenuItems.RemoveAt(i);
                        if ((--j) < 0)
                            break;
                    }
                }
            }
            toolbarMenuItems.Clear();
            updateAppBarModeAndVisibility();
        }

        public void toolbarShow()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarShow(); }); return; }
            updateAppBarModeAndVisibility();
        }

        public void toolbarHide()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarHide(); }); return; }
            updateAppBarModeAndVisibility(false);
        }

        private int toolbarIsStartedFunc()
        {
            return ApplicationBar.Buttons.Count > 0 ? 1 : 0;
        }

        public int toolbarIsStarted()
        {
            return IntValueReturnAgent(toolbarIsStartedFunc);
        }

        private int toolbarGetHeightFunc()
        {
            return ApplicationBar.IsVisible ? (ApplicationBar.Mode == ApplicationBarMode.Default ? 72 : 30) : 0;
        }

        public int toolbarGetHeight()
        {
            return IntValueReturnAgent(toolbarGetHeightFunc);
        }

        public void toolbarAddAction(string text)
        {
            toolbarAddAction(null, text, text);
        }

        public void toolbarAddAction(string icon, string text, string action)
        {
            if ((action == null) || (action.Length == 0))
                return;
            if ((text == null) || (text.Length == 0))
            {
                text = action;
                if (text.IndexOf('/') >= 0)
                {
                    while (text.EndsWith("/"))
                        text.Remove(text.Length - 1);
                    if (text.LastIndexOf('/') >= 0)
                        text = text.Substring(text.LastIndexOf('/') + 1);
                }
            }
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddAction(icon, text, action); }); return; }
            if (ApplicationBar.Buttons.Count < 4)
            {
                ApplicationBarIconButton toolbarButton = new ApplicationBarIconButton();
                toolbarButton.IconUri = new Uri((icon != null) && (icon.Length > 0) ? prependWithSlash(icon) : "/rho/apps/public/images/cancel.png", UriKind.Relative);
                toolbarButton.Text = text;
                ApplicationBar.Buttons.Add(toolbarButton);
                toolbarButton.Click += new EventHandler(toolbarButton_Click);
            }
            else
            {
                ApplicationBarMenuItem menuItem = new ApplicationBarMenuItem(text);
                ApplicationBar.MenuItems.Add(menuItem);
                menuItem.Click += new EventHandler(toolbarMenuItem_Click);
                toolbarMenuItems.Add(menuItem);
            }
            toolbarItems.Add(text, action);
            updateAppBarModeAndVisibility();
        }
        
        public void toolbarAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddSeparator(); }); return; }
            CRhoRuntime.getInstance().logEvent("Toolbar separator is unimplemented on WP8");
        }

        public void setToolbarStyle(bool border, string background, string mask)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { setToolbarStyle(border, background, mask); }); return; }
            // TODO: implement setToolbarStyle
            if (background != "")
                ApplicationBar.BackgroundColor = getColorFromString(background);
            // TODO: implement opacity for pictures
            //if(mask != "")
            //    ApplicationBar.ForegroundColor = getColorFromString(mask);
        }

        private void toolbarButton_Click(object sender, EventArgs e)
        {
            CRhoRuntime.getInstance().onToolbarAction(toolbarItems[(sender as ApplicationBarIconButton).Text]);
        }

        private void toolbarMenuItem_Click(object sender, EventArgs e)
        {
            CRhoRuntime.getInstance().onToolbarAction(toolbarItems[(sender as ApplicationBarMenuItem).Text]);
        }


        // *** MENU ***

        public void menuClear()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuClear(); }); return; }
            ApplicationBar.MenuItems.Clear();
            // restore menu items converted from toolbar buttons (wp8 allows 4 actual buttons only)
            toolbarMenuItems.ForEach(item => ApplicationBar.MenuItems.Add(item));
            menuItems.Clear();
            updateAppBarModeAndVisibility();
        }

        public void menuAddAction(string label, int item)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddAction(label, item); }); return; }
            ApplicationBarMenuItem menuItem = new ApplicationBarMenuItem(label);
            ApplicationBar.MenuItems.Add(menuItem);
            menuItem.Click += delegate(object sender, EventArgs e) { menuItem_Click(sender, e, item); };
            updateAppBarModeAndVisibility();
        }

        public void menuAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddSeparator(); }); return; }
            CRhoRuntime.getInstance().logEvent("Menu separator is unimplemented on WP8");
        }

        private void menuItem_Click(object sender, EventArgs e, int index)
        {
            CRhoRuntime.getInstance().onCustomMenuItemCommand(index);
        }


        // *** TABBAR ***

        public void tabbarInitialize()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarInitialize(); }); return; }
            tabbarRemoveAllTabs();
            // TODO: clear style of tabBar!
        }

        public void tabbarRemoveAllTabs()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarRemoveAllTabs(); }); return; }
            TabbarPivot.Items.Clear();
            _tabProps.Clear();
            _oTabResult = null;
        }

        public void tabbarRemove(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarRemove(index); }); return; }
            TabbarPivot.Items.RemoveAt(index);
        }

        public void tabbarShow()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarShow(); }); return; }
            LayoutRoot.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Star);
            LayoutRoot.RowDefinitions[1].Height = new GridLength(0, GridUnitType.Pixel);
            // not needed: TabbarPanel.Visibility = System.Windows.Visibility.Visible;
        }

        public void tabbarHide()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarHide(); }); return; }
            LayoutRoot.RowDefinitions[0].Height = new GridLength(0, GridUnitType.Pixel);
            LayoutRoot.RowDefinitions[1].Height = new GridLength(1, GridUnitType.Star);
            tabbarRemoveAllTabs();
            // not needed: TabbarPanel.Visibility = System.Windows.Visibility.Collapsed;
        }

        private int tabbarIsStartedFunc()
        {
            return TabbarPivot.Items.Count > 0 ? 1 : 0;
        }

        public int tabbarIsStarted()
        {
            return IntValueReturnAgent(tabbarIsStartedFunc);
        }

        private int tabbarGetHeightFunc()
        {
            return TabbarPivot.Items.Count > 0 ? (int)(TabbarPivot.ActualHeight - ((WebBrowser)TabbarPivot.SelectedItem).ActualHeight) : 0;
        }

        public int tabbarGetHeight()
        {
            return IntValueReturnAgent(tabbarGetHeightFunc);
        }

        public void tabbarSwitch(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarSwitch(index); }); return; }
            if ((index >= 0) && (index < TabbarPivot.Items.Count))
            {
                //raiseTabEvent("onTabFocus", TabbarPivot.SelectedIndex, index);
                TabbarPivot.SelectedIndex = index;
            }
        }

        private int tabbarGetCurrentFunc()
        {
            return TabbarPivot.Items.Count > 0 ? TabbarPivot.SelectedIndex : 0;
        }

        public int tabbarGetCurrent()
        {
            return IntValueReturnAgent(tabbarGetCurrentFunc);
        }

        // TODO: in create support: place_tabs_bottom, background_color, on_change_tab_callback

        private static Color getColorFromString(string strColor)
        {
            if (strColor == null || strColor == "")
                return Color.FromArgb(255, 0, 0, 0);

            int c = Convert.ToInt32(strColor);

            int cR = (c & 0xFF0000) >> 16;
            int cG = (c & 0xFF00) >> 8;
            int cB = (c & 0xFF);

            return Color.FromArgb(255, Convert.ToByte(cR), Convert.ToByte(cG), Convert.ToByte(cB));
        }

        public void tabbarAddTab(string label, string icon, string action, bool disabled, string web_bkg_color, string selected_color, string background_color, bool reload, bool use_current_view_for_tab, bool hasCallback, IMethodResult oResult)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarAddTab(label, icon, action, disabled, web_bkg_color, selected_color, background_color, reload, use_current_view_for_tab, hasCallback, oResult); }); return; }
            PivotItem tab = new PivotItem();
            TabHeader th = new TabHeader();
            th.Label = label;
            th.Background = th.UnselectedBackground = new SolidColorBrush(
                (background_color != null) && (background_color.Length > 0) ?
                getColorFromString(background_color) :
                Color.FromArgb(255, 0, 0, 0)
                );
            th.SelectedBackground = (selected_color != null) && (selected_color.Length > 0) ?
                new SolidColorBrush(getColorFromString(selected_color)) :
                th.UnselectedBackground;
            th.IconImage = (icon == null) || (icon.Length == 0) ?
                new BitmapImage() : new BitmapImage(new Uri(prependWithSlash(icon), UriKind.Relative));
            tab.Header = th;

            _tabProps[TabbarPivot.Items.Count] = new TabProps();
            _tabProps[TabbarPivot.Items.Count]._isReload = reload;
            if (hasCallback == true)
                _oTabResult = oResult;

            // TODO: implement useCurrentViewForTab
            // throws expection:
            //if (use_current_view_for_tab)
            //{
            //    tab.Content = RhodesWebBrowser;
            //}
            //else
            //{

            WebBrowser wv = new WebBrowser();
            wv.Height = double.NaN;
            wv.Width = double.NaN;
            wv.IsScriptEnabled = true;
            wv.SizeChanged += RhodesWebBrowser_SizeChanged;
            wv.Navigated += RhodesWebBrowser_Navigated;
            wv.NavigationFailed += RhodesWebBrowser_NavigationFailed;
            wv.LoadCompleted += RhodesWebBrowser_LoadCompleted;
            wv.Loaded += RhodesWebBrowser_Loaded;
            wv.Unloaded += RhodesWebBrowser_Unloaded;
            wv.ScriptNotify += RhodesWebBrowser_JSNotify;
            wv.Source = new Uri("about:blank");
            wv.TabIndex = TabbarPivot.Items.Count;
            if ((web_bkg_color != null) && (web_bkg_color.Length > 0))
                wv.Background = new SolidColorBrush(getColorFromString(web_bkg_color));
            //wv.SetValue(FrameworkElement.NameProperty, "tabWeb" + TabbarPivot.Items.Count.ToString());
            //if ((action != null) && (action.Length > 0))
            //    wv.Navigate(new Uri(CRhoRuntime.getInstance().canonicalizeRhoUrl(action)));
            tab.Content = wv;
            //}

            tab.IsEnabled = !disabled;
            tab.Tag = action;
            //tab.SetValue(FrameworkElement.NameProperty, "tabItem" + TabbarPivot.Items.Count.ToString());

            TabbarPivot.Items.Add(tab);
        }

        private void TabbarPivot_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            if ((TabbarPivot.Items.Count > 0) && (TabbarPivot.SelectedIndex >= 0) && (TabbarPivot.SelectedIndex < TabbarPivot.Items.Count))
            {
                for (int iTab = 0; iTab < TabbarPivot.Items.Count; ++iTab)
                {
                    TabHeader th = (TabHeader)((PivotItem)TabbarPivot.Items[iTab]).Header;
                    Brush newColor = iTab == TabbarPivot.SelectedIndex ? th.SelectedBackground : th.UnselectedBackground;
                    th.Background = newColor;
                }
                if ((_tabProps.Count > 0) && _tabProps.ContainsKey(TabbarPivot.SelectedIndex) &&
                    ((_tabProps[TabbarPivot.SelectedIndex]._isLoaded == false) ||
                     (_tabProps[TabbarPivot.SelectedIndex]._isLoaded == true && _tabProps[TabbarPivot.SelectedIndex]._isReload == true)))
                {
                    string action = ((string)((PivotItem)TabbarPivot.Items[TabbarPivot.SelectedIndex]).Tag);
                    if (action != null)
                        CRhoRuntime.getInstance().onTabbarCurrentChanged(TabbarPivot.SelectedIndex, action);
                }
            }
            int nOldTab = _tabIndex;
            _tabIndex = TabbarPivot.SelectedIndex;
            if ((nOldTab != _tabIndex) && (_tabIndex > -1))
                raiseTabEvent("onTabFocus", nOldTab, _tabIndex);
        }

        public void tabbarSetBadge(int index, string badge)
        {
            //if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarSetBadge(index, badge); }); return; }
            CRhoRuntime.getInstance().logEvent("Tab badge is not implemented on WP8");
        }


        // *** COMMANDS ***

        // this method is an example of application exit technique (see Quit method in App.xaml.cs)
        public void exitCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { exitCommand(); }); return; }
            IsolatedStorageSettings.ApplicationSettings.Save();
            App.Current.Terminate();
        }

        public void navigateBackCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigateBackCommand(); }); return; }
            this.GoBack();
        }

        public void navigateForwardCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigateForwardCommand(); }); return; }
            this.GoForward();
        }

        public void logCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { logCommand(); }); return; }
        }

        public void refreshCommand(int tab_index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { refreshCommand(tab_index); }); return; }
            this.Refresh(tab_index);
        }

        public void takePicture(string callbackUrl)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { takePicture(callbackUrl); }); return; }
        }

        public void selectPicture(string callbackUrl)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { selectPicture(callbackUrl); }); return; }
        }

        public void alertHidePopup()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { alertHidePopup(); }); return; }
        }

        //public void dateTimePicker(CDateTimeMessage *) {}

        //public void takeSignature(void*); // Signature::Params*

        private int isFullscreenFunc()
        {
            return SystemTray.IsVisible ? 0 : 1;
        }

        public bool isFullscreen()
        {
            return IntValueReturnAgent(isFullscreenFunc) != 0;
        }

        public void fullscreenCommand(int fullScreen)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { fullscreenCommand(fullScreen); }); return; }
            SystemTray.IsVisible = fullScreen == 0;
        }

        public void setCookie(string url, string cookie)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { setCookie(url, cookie); }); return; }
            // TODO: implement
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

             //waitEvent.WaitOne();
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

        private void RhodesWebBrowser_JSNotify(object sender, NotifyEventArgs e)
        {
            String answer = CRhoRuntime.getInstance().onJSInvoke(e.Value);
            WebBrowser wb = sender as WebBrowser;
            wb.IsScriptEnabled = true;
            wb.InvokeScript("__rhoNativeApiResult", new string[] { answer });
        }
    }
}
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
using rhoruntime;
using Microsoft.Phone.Shell;
using System.Windows.Media;
using System.Windows.Controls;
using System.IO.IsolatedStorage;
using rhodes.common;

namespace rhodes
{
    public partial class MainPage : PhoneApplicationPage
    {
        // saved singletone instance of MainPage
        static private MainPage _instance = null;
        // saved id of the UI thread
        private int _uiThreadID = -1;
        // rhodes main thread
        private Thread _rhoruntimeThread;
        // internal variables
        private double _screenWidth;
        private double _screenHeight;
        private double _screenPhysicalWidth;
        private double _screenPhysicalHeight;
        private bool _isBrowserInitialized = false;

        //QUICK FIX
        //REDESIGN IT!!! join all this maps into one map with int key and object value
        //(create tabProps object for reload, init uries, loaded states e.t.c)
        private Dictionary<int, bool> tbInitMap = new Dictionary<int, bool>();
        private Dictionary<int, string> tbInitUri = new Dictionary<int, string>();
        private Dictionary<int, bool> tbLoadedMap = new Dictionary<int, bool>();
        //REDESIGN IT!!! join all this maps into one map with int key and object value
        //(create tabProps object for reload, init uries, loaded states e.t.c)

        private string initUri = "";
        private PageOrientation _screenOrientation = PageOrientation.None;
        // menu items hash table
        private Dictionary<string, int> menuItems = new Dictionary<string, int>();
        // toolbar items hash table
        private Dictionary<string, string>   toolbarItems     = new Dictionary<string, string>();
        private List<ApplicationBarMenuItem> toolbarMenuItems = new List<ApplicationBarMenuItem>();

        public bool isBrowserInitialized(int index)
        {
            return (index == -1) ? _isBrowserInitialized : tbInitMap[index];
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
            InitializeComponent();
            ApplicationBar.IsVisible = false;
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
                Thread.Sleep(5000);
                _rhoruntime.onActivate(0);
            } 
            catch (Exception e)
            {
                RhodesWebBrowser.NavigateToString("<html><head><title>Exception</title></head><body>Exception: "+e.Message+"</body></html>");
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

        public string getScreenOrientation()
        {
            if (_screenOrientation == PageOrientation.Portrait ||
                _screenOrientation == PageOrientation.PortraitDown ||
                _screenOrientation == PageOrientation.PortraitUp)
	        {
		        return "portrait";
	        }
	        else if (_screenOrientation == PageOrientation.Landscape ||
                     _screenOrientation == PageOrientation.LandscapeLeft ||
                     _screenOrientation == PageOrientation.LandscapeRight)
	        {
		         return "landscape";
	        }

            return "none";
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
            e.Cancel = CRhoRuntime.getInstance().onBackKeyPress();
        }

        private void updateOrientation(PageOrientation orientation)
        {
            _screenOrientation = orientation;
            updateScreenSize();
        }

        private void updateScreenSize()
        {
            _screenWidth  = Application.Current.Host.Content.ActualWidth;
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
            // TODO: put after-load app code here (e.g. switch to fullscreen if rhoconfig specifies that)
        }

        private string prependWithSlash(string url)
        {
            return url.StartsWith("/") ? url : "/" + url;
        }

        // *** WEBVIEW ***

		public void navigate(string url, int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigate(url, index); }); return; }

            if (url == "") return;

            if (index == -1 && !_isBrowserInitialized)
            {
                initUri = url;
                return;
            }
            else if (index > -1 && tbInitMap[index] == false)
            {
                tbInitUri[index] = url;
                return;
            }


            if (TabbarPivot.Items.Count == 0)
                RhodesWebBrowser.Navigate(new Uri(url));
            else
                ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).Navigate(new Uri(url));
        }

        public string executeScriptFunc(string script, int index)
        {
            string[] codeString = { script };
            if (TabbarPivot.Items.Count == 0)
                return RhodesWebBrowser.InvokeScript("eval", codeString).ToString();
            else
                return ((WebBrowser)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).InvokeScript("eval", codeString).ToString();
        }

        public string executeScript(string script, int index) 
        {
            return StringValueByStringIntReturnAgent(executeScriptFunc, script, index);
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
            String url = getCurrentURLFunc(-1);
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
        }

        private void RhodesWebBrowser_NavigationFailed(object sender, System.Windows.Navigation.NavigationFailedEventArgs e)
        {
            // TODO: WebView NavigationFailed - do we need this?
        }     
           
        private void RhodesWebBrowser_LoadCompleted(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            String url = getCurrentURLFunc(-1);
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
            int index = (sender as WebBrowser).TabIndex;
            if (index > -1 && !tbInitMap[index])
            {
                tbInitMap[index] = true;
                if (tbInitUri.ContainsKey(index))
                    navigate(tbInitUri[index], index);
            }
            else if (TabbarPivot.Items.Count == 0 && !_isBrowserInitialized)
            {
                _isBrowserInitialized = true;
                navigate(initUri, -1);
            }
            else
            {
                if (TabbarPivot.Items.Count > 0 && url.Contains("about:blank") == false)
                {
                    if (tbLoadedMap[index] == false)
                        tbLoadedMap[index] = true;
                }
            }
        }

        private void RhodesWebBrowser_Loaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(-1));
        }


        private void RhodesWebBrowser_Unloaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(-1));
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
            int j = toolbarMenuItems.Count-1;
            if (j >= 0) {
                for (int i = ApplicationBar.MenuItems.Count - 1; i >= 0; i--) {
                    if (ApplicationBar.MenuItems[i].Equals(toolbarMenuItems[j])) {
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
            if ((text == null) || (text.Length == 0)) {
                text = action;
                if (text.IndexOf('/') >= 0) {
                    while (text.EndsWith("/"))
                        text.Remove(text.Length-1);
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
            if(background != "")
                ApplicationBar.BackgroundColor = getColorFromString(background);
            //implement opacity for pictures
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
            tbInitMap.Clear();
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
                TabbarPivot.SelectedIndex = index;
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

        public void tabbarAddTab(string label, string icon, string action, bool disabled, string web_bkg_color, string selected_color, bool reload, bool use_current_view_for_tab)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarAddTab(label, icon, action, disabled, web_bkg_color, selected_color, reload, use_current_view_for_tab); }); return; }
            PivotItem tab = new PivotItem();
            // TODO: make labels font smaller
            // TODO: implement icons
            tab.Header = label;
            //if ((icon == null) || (icon.Length == 0))
            //{
            //    TextBlock tb = new TextBlock();
            //    tb.Text = label;
            //    tb.FontSize = 20;
            //    tb.Height = double.NaN;
            //    tb.Width = double.NaN;
            //    tb.HorizontalAlignment = System.Windows.HorizontalAlignment.Left;
            //    tb.VerticalAlignment = System.Windows.VerticalAlignment.Center;
            //    tb.SetValue(FrameworkElement.NameProperty, "tabLabel"+TabbarPivot.Items.Count.ToString());
            //    //tb.Margin = new Thickness(5, 5, 5, 5);
            //    //tb.TextWrapping = TextWrapping.Wrap;
            //    //tb.TextAlignment = TextAlignment.Left;
            //    tab.Header = tb;
            //}
            // //else
            // //    tab.Header = new RhoTabHeader(label, prependWithSlash(icon));

            // throws expection:
            //if (use_current_view_for_tab)
            //{
            //    
            //    tab.Content = RhodesWebBrowser;
            //}
            //else
            //{
            tbInitMap[TabbarPivot.Items.Count] = false;
            tbLoadedMap[TabbarPivot.Items.Count] = false;
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
            //wv.SetValue(FrameworkElement.NameProperty, "tabWeb" + TabbarPivot.Items.Count.ToString());
            // TODO: reload
            // TODO: web_bkg_color 
            //if ((action != null) && (action.Length > 0))
            //    wv.Navigate(new Uri(CRhoRuntime.getInstance().canonicalizeRhoUrl(action)));
            tab.Content = wv;
            //}

            if ((selected_color != null) && (selected_color.Length > 0))
                tab.Background = new SolidColorBrush(getColorFromString(selected_color));
            tab.IsEnabled = !disabled;
            tab.Tag = action;
            //tab.SetValue(FrameworkElement.NameProperty, "tabItem" + TabbarPivot.Items.Count.ToString());
              
            TabbarPivot.Items.Add(tab);   
             
            //return TabbarPivot.Items.Count-1;          
        }

        private void TabbarPivot_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            if ((TabbarPivot.Items.Count > 0) && (TabbarPivot.SelectedIndex >= 0) && (TabbarPivot.SelectedIndex < TabbarPivot.Items.Count) && (tbLoadedMap[TabbarPivot.SelectedIndex] == false))
                CRhoRuntime.getInstance().onTabbarCurrentChanged(TabbarPivot.SelectedIndex, ((string)((PivotItem)TabbarPivot.Items[TabbarPivot.SelectedIndex]).Tag));
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
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { exitCommand(); } );  return; }
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

		//public void navigateCommand(TNavigateData* nd) {}
		
        public void takePicture(string callbackUrl)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { takePicture(callbackUrl); }); return; }
        }

		 public void selectPicture(string callbackUrl)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { selectPicture(callbackUrl); }); return; }
        }

		//public void alertShowPopup(CAlertParams *) {}

		public void alertHidePopup()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { alertHidePopup(); }); return; }
        }

		//public void dateTimePicker(CDateTimeMessage *) {}
		
        //public void executeCommand(RhoNativeViewRunnable*) {}
		
        //public void executeRunnable(rho::common::IRhoRunnable* pTask) {}
		
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
            
            waitEvent.WaitOne();
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
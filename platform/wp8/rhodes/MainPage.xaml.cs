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
using System.Threading;
using System.Windows;
using Microsoft.Devices;
using Microsoft.Phone.Controls;
using rhodes.Resources;
using rhoruntime;
using Microsoft.Phone.Shell;

namespace rhodes
{
    public partial class MainPage : PhoneApplicationPage
    {
        // saved id of the UI thread
        private int _uiThreadID = -1;
        // rhodes main thread
        private Thread _rhoruntimeThread;
        // internal variables
        private double _screenWidth;
        private double _screenHeight;
        private double _screenPhysicalWidth;
        private double _screenPhysicalHeight;
        // menu items hash table
        private Dictionary<string, int> menuItems = new Dictionary<string, int>();
        // toolbar items hash table
        private Dictionary<string, string> toolbarItems = new Dictionary<string, string>();

        private bool isUIThread
        {
            get { return _uiThreadID == System.Threading.Thread.CurrentThread.ManagedThreadId; }
        }

        public MainPage()
        {
            _uiThreadID = System.Threading.Thread.CurrentThread.ManagedThreadId;
            _screenWidth = Application.Current.Host.Content.ActualWidth;
            _screenHeight = Application.Current.Host.Content.ActualHeight;
            _screenPhysicalHeight = 4; // assuming 4 inches
            _screenPhysicalWidth = ( _screenPhysicalHeight / _screenHeight ) * _screenWidth; // assuming square pixels

            InitializeComponent();
            ApplicationBar.IsVisible = false;
            try
            {
                // create rhodes runtime object
                var _rhoruntime = CRhoRuntime.getInstance(new MainPageWrapper(this));
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
		
        public void bringToFront()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { bringToFront(); }); return; }
            this.bringToFront();
        }

        private void PhoneApplicationPage_BackKeyPress(object sender, System.ComponentModel.CancelEventArgs e)
        {
            e.Cancel = CRhoRuntime.getInstance().onBackKeyPress();
        }

        private void PhoneApplicationPage_OrientationChanged(object sender, OrientationChangedEventArgs e)
        {
            // TODO: implement OrientationChanged handler
        }

        private void PhoneApplicationPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            // TODO: implement application window size changed event hanlder ?
        }

        private void PhoneApplicationPage_Loaded(object sender, RoutedEventArgs e)
        {
            // TODO: put after-load app code here (e.g. switch to fullscreen if rhoconfig specifies that)
        }


        // *** WEBVIEW ***

		public void navigate(string url, int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigate(url, index); }); return; }
            RhodesWebBrowser.Navigate(new Uri(url));
        }

        public void executeScript(string script, int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { executeScript(script, index); }); return; }
            RhodesWebBrowser.InvokeScript(script);
        }

		public void GoBack()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { GoBack(); }); return; }
            if (RhodesWebBrowser.CanGoBack)
                RhodesWebBrowser.GoBack();
        }

		public void GoForward()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { GoForward(); }); return; }
            if (RhodesWebBrowser.CanGoForward)
                RhodesWebBrowser.GoForward();
        }

		public void Refresh(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { Refresh(index); }); return; }
            RhodesWebBrowser.Navigate(new Uri(RhodesWebBrowser.Source.AbsoluteUri, UriKind.Absolute));
            // another possible implementation: RhodesWebBrowser.InvokeScript("eval", "history.go()");
        }

		public bool isStarted()
        {
            // TODO: implement WebView->isStarted
            return true;
        }

        public string getCurrentURL(int index)
        {
            return RhodesWebBrowser.Source.AbsoluteUri.ToString();
        }

        private void RhodesWebBrowser_Navigated(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(RhodesWebBrowser.Source.AbsoluteUri.ToString());
        }

        private void RhodesWebBrowser_NavigationFailed(object sender, System.Windows.Navigation.NavigationFailedEventArgs e)
        {
            // TODO: WebView NavigationFailed - do we need this?
        }

        private void RhodesWebBrowser_LoadCompleted(object sender, System.Windows.Navigation.NavigationEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(RhodesWebBrowser.Source.AbsoluteUri.ToString());
        }

        private void RhodesWebBrowser_Loaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(RhodesWebBrowser.Source.AbsoluteUri.ToString());
        }

        private void RhodesWebBrowser_Unloaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(RhodesWebBrowser.Source.AbsoluteUri.ToString());
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

		public int toolbarGetHeight()
        {
            return ApplicationBar.IsVisible ? (ApplicationBar.Mode == ApplicationBarMode.Default ? 72 : 30) : 0;
        }

        public void toolbarAddAction(string text)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddAction(text); }); return; }
            ApplicationBarIconButton toolbarButton = new ApplicationBarIconButton();
            // TODO: icon
            toolbarButton.IconUri = new Uri("/rho/public/images/cancel.png", UriKind.Relative);
            toolbarButton.Text = text;
            toolbarItems.Add(text, text);
            ApplicationBar.Buttons.Add(toolbarButton);
            toolbarButton.Click += new EventHandler(toolbarButton_Click);
            updateAppBarModeAndVisibility();
        }

		public void toolbarAddAction(Icon icon, string text, string action, bool rightAlign)
        {
            // TODO: implement custom toolbar action
            toolbarAddAction(text);
            //toolbarItems.Add(text, action);
        }

		public void toolbarAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddSeparator(); }); return; }
            CRhoRuntime.getInstance().logEvent("Toolbar separator is unimplemented on WP8");
        }

		public void setToolbarStyle(bool border, string background)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { setToolbarStyle(border, background); }); return; }
            // TODO: implement setToolbarStyle
        }

        private void toolbarButton_Click(object sender, EventArgs e)
        {
            CRhoRuntime.getInstance().onToolbarAction(toolbarItems[(sender as ApplicationBarIconButton).Text]);
        }


		// *** MENU ***

		public void menuClear()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuClear(); }); return; }
            ApplicationBar.MenuItems.Clear();
            menuItems.Clear();
            updateAppBarModeAndVisibility();
        }

		public void menuAddAction(string text, int item)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddAction(text, item); }); return; }
            ApplicationBarMenuItem menuItem = new ApplicationBarMenuItem(text);
            ApplicationBar.MenuItems.Add(menuItem);
            menuItems.Add(text, item);
            menuItem.Click += new EventHandler(menuItem_Click);
            updateAppBarModeAndVisibility();
        }

		public void menuAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddSeparator(); }); return; }
            CRhoRuntime.getInstance().logEvent("Menu separator is unimplemented on WP8");
        }

        private void menuItem_Click(object sender, EventArgs e)
        {
            CRhoRuntime.getInstance().onCustomMenuItemCommand(menuItems[(sender as ApplicationBarMenuItem).Text]);
        }


        // *** TABBAR ***
		public void tabbarInitialize()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarInitialize(); }); return; }
            // TODO: implement
        }

		public void tabbarRemoveAllTabs(bool restore)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarRemoveAllTabs(restore); }); return; }
            // TODO: implement
        }

		public void tabbarShow()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarShow(); }); return; }
            // TODO: implement
        }

		public void tabbarHide()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarHide(); }); return; }
            // TODO: implement
        }

		public int tabbarGetHeight()
        {
            // TODO: implement
            return 0;
        }

		public void tabbarSwitch(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarSwitch(index); }); return; }
            // TODO: implement
        }

		public int tabbarGetCurrent()
        {
            // TODO: implement
            return 0;
        }

		//public int tabbarAddTab(const String^ label, const char* icon, bool disabled, const Color^ web_bkg_color, QTabBarRuntimeParams& tbri) { return 0; }
		
        public void tabbarSetBadge(int index, string badge)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarSetBadge(index, badge); }); return; }
            // TODO: implement
        }


        // *** COMMANDS ***

        // this method is an example of application exit technique (see Quit method in App.xaml.cs)
        public void exitCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { exitCommand(); } );  return; }
            // TODO: App.Current.Terminate(); -- we may terminate all processes instead of crashing app via Exception ?
            App.Quit();
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
            // TODO: implement
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
            // TODO: implement
        }

		public void selectPicture(string callbackUrl)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { selectPicture(callbackUrl); }); return; }
            // TODO: implement
        }

		//public void alertShowPopup(CAlertParams *) {}

		public void alertHidePopup()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { alertHidePopup(); }); return; }
            // TODO: implement
        }

		//public void dateTimePicker(CDateTimeMessage *) {}
		
        //public void executeCommand(RhoNativeViewRunnable*) {}
		
        //public void executeRunnable(rho::common::IRhoRunnable* pTask) {}
		
        //public void takeSignature(void*); // Signature::Params*

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
    }
}
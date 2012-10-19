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
using System.Threading;
using System.Windows;
using Microsoft.Devices;
using Microsoft.Phone.Controls;
using rhodes.Resources;
using rhoruntime;

namespace rhodes
{
    public partial class MainPage : PhoneApplicationPage, IMainPage
    {
        // saved id of the UI thread
        private int _uiThreadID = -1;
        // rhodes main thread
        private Thread _rhoruntimeThread;
        // internal variables
        double _screenWidth;
        double _screenHeight;
        double _screenPhysicalWidth;
        double _screenPhysicalHeight;

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
            try
            {
                // create rhodes runtime object
                var rhoruntime = new CRhoRuntime(this);
                // create and start rhodes main thread
                _rhoruntimeThread = new Thread(rhoruntime.Execute);
                _rhoruntimeThread.Start();
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
		

        // *** WEBVIEW ***

		public void navigate(string url, int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigate(url, index); }); return; }
            RhodesWebBrowser.Navigate(new Uri(url));
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
            // TODO: implement
            return true;
        }

        
        // *** TOOLBAR ***

		public void toolbarRemoveAllButtons()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarRemoveAllButtons(); }); return; }
            // TODO: implement
        }

		public void toolbarShow()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarShow(); }); return; }
            // TODO: implement
        }

		public void toolbarHide()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarHide(); }); return; }
            // TODO: implement
        }

		public int toolbarGetHeight()
        {
            // TODO: implement
            return 0;
        }

        public void toolbarAddAction(string text)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddAction(text); }); return; }
            // TODO: implement
        }

		//void toolbarAddAction(const Icon^ icon, const String^ text, const char* action, bool rightAlign /*= false*/) { }

		public void toolbarAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddSeparator(); }); return; }
            // TODO: implement
        }

		public void setToolbarStyle(bool border, string background)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { setToolbarStyle(border, background); }); return; }
            // TODO: implement
        }


		// *** MENU ***

		public void menuClear()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuClear(); }); return; }
            // TODO: implement
        }

		public void menuAddAction(string text, int item)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddAction(text, item); }); return; }
            // TODO: implement
        }

		public void menuAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddSeparator(); }); return; }
            // TODO: implement
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
            // TODO: implement
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
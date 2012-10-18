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

        private bool isUIThread
        {
            get { return _uiThreadID == System.Threading.Thread.CurrentThread.ManagedThreadId; }
        }

        public MainPage()
        {
            _uiThreadID = System.Threading.Thread.CurrentThread.ManagedThreadId;
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
            return 0;
        }
		
        public int getLogicalDpiY()
        {
            return 0;
        }
		
        public void bringToFront()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { bringToFront(); }); return; }
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
            RhodesWebBrowser.InvokeScript("eval", "history.go()");
        }

		public bool isStarted()
        {
            return true;
        }

        
        // *** TOOLBAR ***

		public void toolbarRemoveAllButtons()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarRemoveAllButtons(); }); return; }
        }

		public void toolbarShow()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarShow(); }); return; }
        }

		public void toolbarHide()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarHide(); }); return; }
        }

		public int toolbarGetHeight()
        {
            return 0;
        }

        public void toolbarAddAction(string text)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddAction(text); }); return; }
        }

		//void toolbarAddAction(const Icon^ icon, const String^ text, const char* action, bool rightAlign /*= false*/) { }

		public void toolbarAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { toolbarAddSeparator(); }); return; }
        }

		public void setToolbarStyle(bool border, string background)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { setToolbarStyle(border, background); }); return; }
        }


		// *** MENU ***

		public void menuClear()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuClear(); }); return; }
        }

		public void menuAddAction(string text, int item)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddAction(text, item); }); return; }
        }

		public void menuAddSeparator()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { menuAddSeparator(); }); return; }
        }


        // *** TABBAR ***
		public void tabbarInitialize()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarInitialize(); }); return; }
        }

		public void tabbarRemoveAllTabs(bool restore)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarRemoveAllTabs(restore); }); return; }
        }

		public void tabbarShow()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarShow(); }); return; }
        }

		public void tabbarHide()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarHide(); }); return; }
        }

		public int tabbarGetHeight()
        {
            return 0;
        }

		public void tabbarSwitch(int index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarSwitch(index); }); return; }
        }

		public int tabbarGetCurrent()
        {
            return 0;
        }

		//public int tabbarAddTab(const String^ label, const char* icon, bool disabled, const Color^ web_bkg_color, QTabBarRuntimeParams& tbri) { return 0; }
		
        public void tabbarSetBadge(int index, string badge)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { tabbarSetBadge(index, badge); }); return; }
        }


        // *** COMMANDS ***

        // this method is an example of application exit technique (see Quit method in App.xaml.cs)
        public void exitCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { exitCommand(); } );  return; }
            // exit application
            App.Quit();
        }

        public void navigateBackCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigateBackCommand(); }); return; }
        }

		public void navigateForwardCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { navigateForwardCommand(); }); return; }
        }

		public void logCommand()
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { logCommand(); }); return; }
        }

		public void refreshCommand(int tab_index)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { refreshCommand(tab_index); }); return; }
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

		public void fullscreenCommand(int fullScreen)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { fullscreenCommand(fullScreen); }); return; }
        }

		public void setCookie(string url, string cookie)
        {
            if (!isUIThread) { Dispatcher.BeginInvoke(delegate() { setCookie(url, cookie); }); return; }
        }


        // *** MISC ***
        // this method is used as a callback for calling the C# API methods from C++
        public void DoWait(int timeout)
        {
            Thread.Sleep(timeout);
        }

    }
}
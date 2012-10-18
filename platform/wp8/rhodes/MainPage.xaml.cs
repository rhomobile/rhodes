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

        // this method is used as a callback for calling the C# API methods from C++
        public void DoWait(int timeout)
        {
            Thread.Sleep(timeout);
        }

        /*
        // this method is used for updating UI controls from C++ non-UI thread
        public void UpdateWebView(int counter)
        {
            // perform this action on UI thread only
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { UpdateWebView(counter); } );  return; }
            // update WebView
            RhodesWebBrowser.NavigateToString("<html><head><title>Loading...</title></head><body>" + counter + "<input type=\"text\" name=\"test\" size=\"25\"/></body></html>");
        }
        */

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
        }
		

        // *** WEBVIEW ***

		public void navigate(string url, int index)
        {
        }

		public void GoBack()
        {
        }

		public void GoForward()
        {
        }

		public void Refresh(int index)
        {
        }

		public bool isStarted()
        {
            return true;
        }

        
        // *** TOOLBAR ***

		public void toolbarRemoveAllButtons()
        {
        }

		public void toolbarShow()
        {
        }

		public void toolbarHide()
        {
        }

		public int toolbarGetHeight()
        {
            return 0;
        }

        public void toolbarAddAction(string text)
        {
        }

		//void toolbarAddAction(const Icon^ icon, const String^ text, const char* action, bool rightAlign /*= false*/) { }

		public void toolbarAddSeparator()
        {
        }

		public void setToolbarStyle(bool border, string background)
        {
        }


		// *** MENU ***

		public void menuClear()
        {
        }

		public void menuAddAction(string text, int item)
        {
        }

		public void menuAddSeparator()
        {
        }


        // *** TABBAR ***
		public void tabbarInitialize()
        {
        }

		public void tabbarRemoveAllTabs(bool restore)
        {
        }

		public void tabbarShow()
        {
        }

		public void tabbarHide()
        {
        }

		public int tabbarGetHeight()
        {
            return 0;
        }

		public void tabbarSwitch(int index)
        {
        }

		public int tabbarGetCurrent()
        {
            return 0;
        }

		//public int tabbarAddTab(const String^ label, const char* icon, bool disabled, const Color^ web_bkg_color, QTabBarRuntimeParams& tbri) { return 0; }
		
        public void tabbarSetBadge(int index, string badge)
        {
        }


        // *** COMMANDS ***

        // this method is an example of application exit technique (see Quit method in App.xaml.cs)
        public void exitCommand()
        {
            // perform this action on UI thread only
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { exitCommand(); } );  return; }
            // exit application
            App.Quit();
        }

        public void navigateBackCommand()
        {
        }

		public void navigateForwardCommand()
        {
        }

		public void logCommand()
        {
        }

		public void refreshCommand(int tab_index)
        {
        }

		//public void navigateCommand(TNavigateData* nd) {}
		
        public void takePicture(string callbackUrl)
        {
        }

		public void selectPicture(string callbackUrl)
        {
        }

		//public void alertShowPopup(CAlertParams *) {}

		public void alertHidePopup()
        {
        }

		//public void dateTimePicker(CDateTimeMessage *) {}
		
        //public void executeCommand(RhoNativeViewRunnable*) {}
		
        //public void executeRunnable(rho::common::IRhoRunnable* pTask) {}
		
        //public void takeSignature(void*); // Signature::Params*

		public void fullscreenCommand(int fullScreen)
        {
        }

		public void setCookie(string url, string cookie)
        {
        }

    }
}
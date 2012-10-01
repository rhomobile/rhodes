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

        // this method is used for updating UI controls from C++ non-UI thread
        public void UpdateWebView(int counter)
        {
            // perform this action on UI thread only
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { UpdateWebView(counter); } );  return; }
            // update WebView
            RhodesWebBrowser.NavigateToString("<html><head><title>Loading...</title></head><body>" + counter + "<input type=\"text\" name=\"test\" size=\"25\"/></body></html>");
        }

        // this method is an example of application exit technique (see Quit method in App.xaml.cs)
        public void DoExit()
        {
            // perform this action on UI thread only
            if (!isUIThread) { Dispatcher.BeginInvoke( delegate() { DoExit(); } );  return; }
            // exit application
            App.Quit();
        }
    }
}
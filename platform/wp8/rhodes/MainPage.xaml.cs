using System;
using System.Threading;
using Microsoft.Phone.Controls;
using rhodes.Resources;
using rhoruntime;

namespace rhodes
{
    public partial class MainPage : PhoneApplicationPage
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
                var rhoruntime = new CRhoRuntime();
                
                // set callback methods
                rhoruntime.SetWaitCallback(new WaitCallbackPointer(this.DoWait));
                rhoruntime.SetUpdateWebViewCallback(new UpdateWebViewCallbackPointer(this.UpdateWebView));
                rhoruntime.SetExitCallback(new ExitCallbackPointer(this.DoExit));
                
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
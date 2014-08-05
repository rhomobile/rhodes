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
using Windows.ApplicationModel.Core;
using Windows.UI.Core;
using System.Text.RegularExpressions;
using Windows.Graphics.Display;
using System.Threading.Tasks;
using System.IO.Compression;
using Windows.Storage;
using Windows.Storage.Streams;

namespace rhodes
{
    public sealed partial class MainPage : Page
    {
        // saved singletone instance of MainPage
        static private MainPage _instance = null;
        // saved id of the UI thread
        private int _uiThreadID = -1;
        // rhodes main thread
        private IAsyncAction _rhoruntimeThread;
        //private bool _isCallbackFired = false;
        // internal variables
        private double _screenWidth;
        private double _screenHeight;
        private double _screenPhysicalWidth;
        private double _screenPhysicalHeight;
        private bool _isBrowserInitialized = false;
        //private int _tabIndex = -1;
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
        private string _userAgent = "Unknown";
        private string _OSVersion = "8.0";

        private string initUri = "";
        private DisplayOrientations _screenOrientation = DisplayOrientations.None;

        public bool isBrowserInitialized(int index)
        {
            return _isBrowserInitialized; // (index == -1) || !_tabProps.ContainsKey(index) ? _isBrowserInitialized : _tabProps[index]._isInitialized;
        }

        private bool isUIThread
        {
            get { return _uiThreadID == Environment.CurrentManagedThreadId; }
        }

        static public MainPage getInstance()
        {
            return _instance;
        }

        public MainPage()
        {
            _instance = this;
            _uiThreadID = Environment.CurrentManagedThreadId;
            updateScreenSize();
            this.InitializeComponent();
            RhodesWebBrowser.AllowedScriptNotifyUris = WebView.AnyScriptNotifyUri;
            DisplayProperties.OrientationChanged += DisplayProperties_OrientationChanged;
            // TODO: ApplicationBar.IsVisible = false;
            _screenOrientation = DisplayProperties.CurrentOrientation;
            unpackAppToLocalStorage();
            try
            {
                // TODO: initialize C# extensions factories
                //CSharpExtensions.InitializeExtensions();
                // create rhodes runtime object
                var _rhoruntime = CRhoRuntime.getInstance(new MainPageWrapper(this));
                // TODO: _rhoruntime.setCryptoEngine(new CryptoEngineWrapper(new RhoCrypt()));
                // create and start rhodes main thread               
                _rhoruntimeThread = Windows.System.Threading.ThreadPool.RunAsync(
                    (workItem) =>
                    {
                        _rhoruntime.Execute();
                    }
                 );
                _rhoruntime.onActivate(0);
            }
            catch (Exception e)
            {
                RhodesWebBrowser.NavigateToString("<html><head><title>Exception</title></head><body>Exception: " + e.Message + "</body></html>");
            }
        }

        private async Task unpackAppToLocalStorage()
        {
            // open zip file stream
            var _Folder = Windows.ApplicationModel.Package.Current.InstalledLocation;
            var _File = await _Folder.GetFileAsync("rho.zip");
            var randomAccessStream = await _File.OpenReadAsync();
            Stream stream = randomAccessStream.AsStreamForRead();

            // unzip into local storage root
            StorageFolder unZipfolder = ApplicationData.Current.LocalFolder;
            var zipArchive = new ZipArchive(stream, ZipArchiveMode.Read);
            foreach (var zipArchiveEntry in zipArchive.Entries)
            {
                if (!String.IsNullOrEmpty(zipArchiveEntry.FullName))
                {
                    if (zipArchiveEntry.Name == "")
                    {
                        System.Diagnostics.Debug.WriteLine("Creating folder " + zipArchiveEntry.FullName);
                        await CreateRecursiveFolder(unZipfolder, zipArchiveEntry);
                    }
                    else
                    {
                        System.Diagnostics.Debug.WriteLine("Unpacking " + zipArchiveEntry.FullName);
                        await ExtractFile(unZipfolder, zipArchiveEntry);
                    }
                }
            }
        }

        private async Task CreateRecursiveFolder(StorageFolder folder, ZipArchiveEntry entry)
        {
            var steps = entry.FullName.Split('/').ToList();
            steps.RemoveAt(steps.Count() - 1);
            foreach (var i in steps)
            {
                await folder.CreateFolderAsync(i, CreationCollisionOption.OpenIfExists);
                folder = await folder.GetFolderAsync(i);
            }
        }

        private async Task ExtractFile(StorageFolder folder, ZipArchiveEntry entry)
        {
            var steps = entry.FullName.Split('/').ToList();
            steps.RemoveAt(steps.Count() - 1);
            foreach (var i in steps)
                folder = await folder.GetFolderAsync(i);

            using (Stream fileData = entry.Open())
            {
                StorageFile outputFile = await folder.CreateFileAsync(entry.Name, CreationCollisionOption.ReplaceExisting);
                using (Stream outputFileStream = await outputFile.OpenStreamForWriteAsync())
                {
                    await fileData.CopyToAsync(outputFileStream);
                    await outputFileStream.FlushAsync();
                    outputFileStream.Dispose();
                }
            }
        }

        private void DisplayProperties_OrientationChanged(object sender)
        {
            updateOrientation(DisplayProperties.CurrentOrientation);
        }

        private void RhodesWebBrowser_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            // TODO: webview size changed event handler ?
        }

        private void RhodesWebBrowser_NavigationFailed(object sender, WebViewNavigationFailedEventArgs e)
        {
            // TODO: WebView NavigationFailed - do we need this?
        }

        private void RhodesWebBrowser_LoadCompleted(object sender, NavigationEventArgs e)
        {
            String url = getCurrentURLFunc(0);//(sender as WebView).TabIndex);
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
            if (!_isBrowserInitialized)
            {
                _isBrowserInitialized = true;
                navigate(initUri, -1);
            }
        }

        private void RhodesWebBrowser_Loaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(0)); // (sender as WebView).TabIndex
        }

        private void RhodesWebBrowser_Unloaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(0)); // (sender as WebView).TabIndex
        }

        private void RhodesWebBrowser_ScriptNotify(object sender, NotifyEventArgs e)
        {
            String answer = CRhoRuntime.getInstance().onJSInvoke(e.Value);
            WebView wb = sender as WebView;
            wb.InvokeScript("__rhoNativeApiResult", new string[] { answer });
        }

        private void Page_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            updateScreenSize();
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            var browser = new WebView();
            browser.AllowedScriptNotifyUris = WebView.AnyScriptNotifyUri;
            browser.Visibility = Visibility.Collapsed;
            browser.Loaded += (asender, args) => browser.NavigateToString(getUserAgentHtml);
            browser.ScriptNotify += (asender, args) =>
            {
                string userAgent = args.Value;
                try
                {
                    var match = Regex.Match(userAgent, @"Windows\s+NT\s+(\d+\.\d+)?");
                    if (match.Success)
                        this._OSVersion = match.Value;
                }
                catch (Exception) { }
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
            return "IE/" + _OSVersion;
        }

        internal string getScreenOrientation()
        {
            if (_screenOrientation == DisplayOrientations.Portrait ||
                _screenOrientation == DisplayOrientations.PortraitFlipped ||
                _screenOrientation == DisplayOrientations.None)
            {
                return "portrait";
            }
            else
            //if (_screenOrientation == DisplayOrientations.Landscape ||
            //    _screenOrientation == DisplayOrientations.LandscapeFlipped)
            {
                return "landscape";
            }
        }

        internal async void bringToFront()
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { bringToFront(); }); return; }
            this.bringToFront();
        }

        internal async void performOnUiThread(long native)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { performOnUiThread(native); }); return; }
            CRhoRuntime.getInstance().executeRhoRunnable(native);
        }

        private void updateOrientation(DisplayOrientations orientation)
        {
            _screenOrientation = orientation;
            updateScreenSize();
        }

        private void updateScreenSize()
        {
            double dpi = DisplayProperties.LogicalDpi;
            var bounds = Window.Current.Bounds;
            _screenWidth = bounds.Width;
            _screenHeight = bounds.Height;
            _screenPhysicalHeight = _screenHeight / dpi;
            _screenPhysicalWidth = _screenWidth / dpi;

            if ((_screenOrientation == DisplayOrientations.Landscape) || (_screenOrientation == DisplayOrientations.LandscapeFlipped))
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

        internal async void navigate(string url, int index)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { navigate(url, index); }); return; }
            RhodesWebBrowser.Navigate(new Uri(url, UriKind.RelativeOrAbsolute));
        }

        public string executeScriptFunc(string script, int index)
        {
            string[] codeString = { script };
            //_isCallbackFired = false;
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
            GoBack(0);//TabbarPivot.SelectedIndex);
        }

        internal async void GoBack(int index)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { GoBack(index); }); return; }
            // TODO: implement history stack
            RhodesWebBrowser.InvokeScript("eval", new[] { "(function(){ history.go(-1);})()" });
        }

        internal void GoForward()
        {
            GoForward(0);//TabbarPivot.SelectedIndex);
        }

        internal async void GoForward(int index)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { GoForward(index); }); return; }
            // TODO: implement history stack
            RhodesWebBrowser.InvokeScript("eval", new[] { "(function(){ history.go(+1);})()" });
        }

        internal async void Refresh(int index)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { Refresh(index); }); return; }
            RhodesWebBrowser.Navigate(new Uri(RhodesWebBrowser.Source.AbsoluteUri, UriKind.Absolute));
        }

        internal bool isStarted()
        {
            // TODO: implement WebView->isStarted
            return true;
        }

        internal bool isEmulator()
        {
            // TODO: simulator detection
            return false;
        }

        private string getCurrentURLFunc(int index)
        {
            try
            {
                return RhodesWebBrowser.Source.ToString();
            }
            catch (Exception)
            {
                return "";
            }
        }

        internal string getCurrentURL(int index)
        {
            return StringValueByIntReturnAgent(getCurrentURLFunc, -1);
        }


        // *** TOOLBAR ***

        internal void toolbarRemoveAllButtons()
        {
            // TODO: implement toolbar
        }

        internal void toolbarShow()
        {
            // TODO: implement toolbar
        }

        internal void toolbarHide()
        {
            // TODO: implement toolbar
        }

        private int toolbarIsStartedFunc()
        {
            // TODO: implement toolbar
            return 0; // ApplicationBar.Buttons.Count > 0 ? 1 : 0;
        }

        internal int toolbarIsStarted()
        {
            return IntValueReturnAgent(toolbarIsStartedFunc);
        }

        private int toolbarGetHeightFunc()
        {
            // TODO: implement toolbar
            return 0; // ApplicationBar.IsVisible ? (ApplicationBar.Mode == ApplicationBarMode.Default ? 72 : 30) : 0;
        }

        internal int toolbarGetHeight()
        {
            return IntValueReturnAgent(toolbarGetHeightFunc);
        }

        internal void toolbarAddAction(string text)
        {
            toolbarAddAction(null, text, text);
        }

        internal void toolbarAddAction(string icon, string text, string action)
        {
            // TODO: implement toolbar
        }

        internal void toolbarAddSeparator()
        {
            // TODO: implement toolbar
        }

        internal void setToolbarStyle(bool border, string background, string mask)
        {
            // TODO: implement toolbar
        }


        // *** MENU ***

        internal void menuClear()
        {
            // TODO: implement menu
        }

        internal void menuAddAction(string text, int item)
        {
            // TODO: implement menu
        }

        internal void menuAddSeparator()
        {
            // TODO: implement menu
        }


        // *** TABBAR ***
        
        internal void tabbarInitialize()
        {
            // TODO: implement tabbar
        }

        internal void tabbarRemoveAllTabs()
        {
            // TODO: implement tabbar
        }

        internal void tabbarShow()
        {
            // TODO: implement tabbar
        }

        internal void tabbarHide()
        {
            // TODO: implement tabbar
        }

        private int tabbarIsStartedFunc()
        {
            // TODO: implement tabbar
            return 0; // TabbarPivot.Items.Count > 0 ? 1 : 0;
        }

        internal int tabbarIsStarted()
        {
            return IntValueReturnAgent(tabbarIsStartedFunc);
        }

        private int tabbarGetHeightFunc()
        {
            // TODO: implement tabbar
            return 0; // TabbarPivot.Items.Count > 0 ? (int)(TabbarPivot.ActualHeight - ((WebBrowser)TabbarPivot.SelectedItem).ActualHeight) : 0;
        }

        internal int tabbarGetHeight()
        {
            throw new NotImplementedException();
        }

        internal void tabbarRemove(int index)
        {
            // TODO: implement tabbar
        }

        internal void tabbarSwitch(int index)
        {
            // TODO: implement tabbar
        }

        private int tabbarGetCurrentFunc()
        {
            // TODO: implement tabbar
            return 0; // TabbarPivot.Items.Count > 0 ? TabbarPivot.SelectedIndex : 0;
        }

        internal int tabbarGetCurrent()
        {
            return IntValueReturnAgent(tabbarGetCurrentFunc);
        }

        internal void tabbarAddTab(string label, string icon, string action, bool disabled, string web_bkg_color, string selected_color, string background_color, bool reload, bool use_current_view_for_tab, bool hasCallback, rhoruntime.IMethodResult oResult)
        {
            // TODO: implement tabbar
        }

        internal void tabbarSetBadge(int index, string badge)
        {
            // TODO: implement tabbar
        }


        // *** COMMANDS ***

        internal async void exitCommand()
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { exitCommand(); }); return; }
            // TODO: WinRT equivalent of IsolatedStorageSettings.ApplicationSettings.Save();
            App.Current.Exit();
        }

        internal void navigateBackCommand()
        {
            this.GoBack();
        }

        internal void navigateForwardCommand()
        {
            this.GoForward();
        }

        internal async void logCommand()
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { logCommand(); }); return; }
            // TODO: implement log command
        }

        internal void refreshCommand(int tab_index)
        {
            this.Refresh(tab_index);
        }

        internal void takePicture(string callbackUrl)
        {
            // TODO: implement log command
        }

        internal void selectPicture(string callbackUrl)
        {
            // TODO: implement log command
        }

        internal void alertHidePopup()
        {
            // TODO: implement log command
        }

        internal void fullscreenCommand(int fullScreen)
        {
            // not applicable for Metro apps since they are always
            //if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { fullscreenCommand(fullScreen); }); return; }
            //SystemTray.IsVisible = fullScreen == 0;
        }

        internal async void setCookie(string url, string cookie)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { setCookie(url, cookie); }); return; }
            // TODO: implement set cookie
        }


        // *** MISC ***

        // this method is used as a callback for calling the C# API methods from C++
        internal async void DoWait(int timeout)
        {
            await Task.Delay(TimeSpan.FromMilliseconds(timeout));
        }

        internal async void DisplayMessage(string msg)
        {
            if (!isUIThread) { await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () => { DisplayMessage(msg); }); return; }
            RhodesWebBrowser.NavigateToString("<html><head><title>Message</title></head><body>" + msg + "</body></html>");
        }

        private int IntValueReturnAgent(Func<int> func)
        {
            if (isUIThread)
                return func();

            Exception exception = null;
            var waitEvent = new System.Threading.ManualResetEvent(false);
            int return_value = 0;
            #pragma warning disable 4014
            CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
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
            #pragma warning restore 4014

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
            #pragma warning disable 4014
            CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
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
            #pragma warning restore 4014

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
            #pragma warning disable 4014
            CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
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
            #pragma warning restore 4014

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
            #pragma warning disable 4014
            CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
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
            #pragma warning restore 4014

            if (exception != null)
                throw exception;

            return return_value;
        }
    }
}

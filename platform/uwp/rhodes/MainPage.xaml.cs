using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using rhodes.Resources;
using rhoruntime;
using System.IO.IsolatedStorage;
using rhodes.common;
using System.ComponentModel;
using Windows.System.Profile;
using Windows.UI.ViewManagement;
using Windows.Devices.Sensors;
using Windows.Devices.Sensors.Custom;
using Windows.UI.Core;
using Windows.Security.ExchangeActiveSyncProvisioning;
using Windows.UI;
using System.Net.Http.Headers;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.Web;
using RhoRuntimeInvoker;
using System.Reflection;
using System.Xml.Linq;
using Windows.ApplicationModel;



namespace rhodes
{
    class WebViewTag
    {
        public WebViewTag(int tabIndex){ this.tabIndex = tabIndex; }
        public int getTabIndex(){ return tabIndex;}
        private int tabIndex = 0;
    }

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

    public partial class MainPage : Page
    {

        static CoreDispatcher dispatcher = null;
        public static CoreDispatcher getDispatcher()
        {
            if (dispatcher == null) { dispatcher = Windows.UI.Core.CoreWindow.GetForCurrentThread().Dispatcher; }
            return dispatcher;
        }
        
        // saved singletone instance of MainPage
        static private MainPage _instance = null;
        private IMethodResult _oTabResult = null;
        // saved id of the UI thread
        private int _uiThreadID = -1;
        // rhodes main thread
        private Task _rhoruntimeThread;
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

        private static readonly Windows.Security.ExchangeActiveSyncProvisioning.EasClientDeviceInformation DEVICE_INFO =
                new Windows.Security.ExchangeActiveSyncProvisioning.EasClientDeviceInformation();

        private string _userAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows Phone " +
            DEVICE_INFO.OperatingSystem + "." +
            DEVICE_INFO.SystemManufacturer +
            "; Trident/6.0; IEMobile/10.0; ARM; Touch; " +
            DEVICE_INFO.SystemFirmwareVersion + "; " +
            DEVICE_INFO.SystemHardwareVersion + ")";
        
        private Dictionary<int, TabProps> _tabProps = new Dictionary<int, TabProps>();

        private string initUri = "";
        private SimpleOrientation _screenOrientation = SimpleOrientation.Faceup;

        // menu items hash table
        private Dictionary<string, int> menuItems = new Dictionary<string, int>();
        // toolbar items hash table
        private Dictionary<string, string> toolbarItems = new Dictionary<string, string>();
        
        private List<AppBarButton> toolbarMenuItems = new List<AppBarButton>();

        public bool isBrowserInitialized(int index)
        {
            return (index == -1) || !_tabProps.ContainsKey(index) ? _isBrowserInitialized : _tabProps[index]._isInitialized;
        }

        private bool isUIThread
        {
            get { return _uiThreadID == Environment.CurrentManagedThreadId; }
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
        /*public UIElement RootVisual()
        {
            DependencyObject parent = VisualTreeHelper.GetParent((DependencyObject)Content);
            return App.Current.RootVisual;
        }*/
        public bool isEmulator()
        {
            return DEVICE_INFO.SystemProductName.Contains("Emulator") == true ? true : false;
        }

        /*private CommandBar getTopAppBar()
        {
            return TopCommandBar;
        }*/

        private CommandBar getBottomAppBar()
        {
            return BottomCommandBar;
            //return BottomAppBar.Content as CommandBar;
        }

        CommandBar getAppBar()
        {
            return getBottomAppBar();
        }

        private static void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs)
        {
            // Get the subdirectories for the specified directory.
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);

            if (!dir.Exists)
            {
                throw new DirectoryNotFoundException(
                    "Source directory does not exist or could not be found: "
                    + sourceDirName);
            }

            DirectoryInfo[] dirs = dir.GetDirectories();
            // If the destination directory doesn't exist, create it.
            if (!Directory.Exists(destDirName))
            {
                Directory.CreateDirectory(destDirName);
            }

            // Get the files in the directory and copy them to the new location.
            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(destDirName, file.Name);
                file.CopyTo(temppath, false);
            }

            // If copying subdirectories, copy them and their contents to new location.
            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }
        private static DirectoryInfo rhoDir = null;
        public static DirectoryInfo getDBDir()
        {
            DirectoryInfo dbDir = new DirectoryInfo(rhoDir.FullName + "\\db\\db-files");
            try
            {
                if (!dbDir.Exists)
                {
                    dbDir.Create();
                }
                return dbDir;
            }catch(Exception e)
            {
                return null;
            }
            
        }
        public MainPage()
        {
            deb("Running constructor");
            StorageFolder appInstalledFolder = Windows.ApplicationModel.Package.Current.InstalledLocation;
            StorageFolder localFolder = ApplicationData.Current.LocalFolder;
            deb("Storage folder is " + appInstalledFolder.Path);
            deb("Local folder is " + localFolder.Path);
            

            String title = Package.Current.DisplayName;

            string deviceFamilyVersion = AnalyticsInfo.VersionInfo.DeviceFamilyVersion;
            ulong version = ulong.Parse(deviceFamilyVersion);
            int major = (int)((version & 0xFFFF000000000000L) >> 48);
            int minor = (int)((version & 0x0000FFFF00000000L) >> 32);
            int build = (int)((version & 0x00000000FFFF0000L) >> 16);
            //int revision = (int)((version & 0x000000000000FFFFL));            

            rhoDir = new DirectoryInfo(localFolder.Path + "\\rho");
            
            if (!rhoDir.Exists)
            {
                DirectoryCopy(appInstalledFolder.Path + "\\rho", rhoDir.FullName, true);
                localFolder.CreateFileAsync("\\rho\\" + title, Windows.Storage.CreationCollisionOption.OpenIfExists).Completed = (arg1, arg2) => {
                    try
                    {
                        System.IO.Directory.SetCurrentDirectory(rhoDir.FullName);
                    }
                    catch (Exception e)
                    {
                        deb("Exception on setting current directiory: " + e.Message);
                    }

                }; ;
            }
            else
            {
                try
                {
                    System.IO.Directory.SetCurrentDirectory(rhoDir.FullName);
                }
                catch (Exception e)
                {
                    deb("Exception on setting current directiory: " + e.Message);
                }
                FileInfo fInfo = new FileInfo(localFolder.Path + "\\rho\\" + title);
                if (fInfo.Exists){
                    deb("Rho exists");
                }
                else{
                    IAsyncOperation<StorageFolder> rhoFolderTask = StorageFolder.GetFolderFromPathAsync(rhoDir.FullName);
                    rhoFolderTask.AsTask().Wait();
                    IAsyncAction action = rhoFolderTask.GetResults().DeleteAsync();
                    try {
                        action.AsTask().Start();
                        action.AsTask().Wait();
                    }
                    catch (Exception e) { }

                    DirectoryCopy(appInstalledFolder.Path + "\\rho", rhoDir.FullName, true);
                    localFolder.CreateFileAsync("\\rho\\" + title, Windows.Storage.CreationCollisionOption.OpenIfExists);
                }
            }
            

            {
                IAsyncOperation<StorageFile> task = localFolder.CreateFileAsync("nullfile", Windows.Storage.CreationCollisionOption.OpenIfExists);
                try
                {
                    task.AsTask().Wait();
                }catch(Exception e){}
            }

            getDispatcher();
            try
            {
                _screenOrientation = SimpleOrientationSensor.GetDefault().GetCurrentOrientation();
                SimpleOrientationSensor.GetDefault().OrientationChanged +=
                new TypedEventHandler<SimpleOrientationSensor, SimpleOrientationSensorOrientationChangedEventArgs>(ApplicationPage_OrientationChanged);
            }
            catch(Exception e){ _screenOrientation = SimpleOrientation.Faceup; }
            _instance = this;
            _uiThreadID = Environment.CurrentManagedThreadId;
            updateScreenSize();
            InitializeComponent();
            RhodesWebBrowser.Tag = new WebViewTag(-1);
            RhodesWebBrowser.Settings.IsJavaScriptEnabled = true;
            RhodesWebBrowser.Settings.IsIndexedDBEnabled = true;


            /*if ((TopAppBar == null) || (TopAppBar.Content == null)) {
                deb("Creating commandBar");
                CommandBar commandBar = new CommandBar();
                if (this.TopAppBar == null) {
                    deb("Creating TopAppBar"); 
                    this.TopAppBar = new AppBar();
                    this.TopAppBar.IsOpen = false;
                    this.TopAppBar.IsSticky = false;
                }
                TopAppBar.Content = commandBar;
            }
            TopAppBar.Visibility = Visibility.Collapsed;

            if ((BottomAppBar == null) || (BottomAppBar.Content == null))
            {
                deb("Creating commandBar");
                CommandBar commandBar = new CommandBar();
                if (this.BottomAppBar == null)
                {
                    deb("Creating BottomAppBar");
                    this.BottomAppBar = new AppBar();
                    this.BottomAppBar.IsOpen = true;
                    this.BottomAppBar.IsSticky = true;
                }
                BottomAppBar.Content = commandBar;
            }
            //BottomAppBar.Visibility = Visibility.Collapsed;*/
            getAppBar().Visibility = Visibility.Collapsed;

            try
            {
                // initialize C# extensions factories
                deb("InitializeExtensions()");
                
                CSharpExtensions.InitializeExtensions();

                // create rhodes runtime object
                var _rhoruntime = CRhoRuntime.getInstance(new MainPageWrapper(this), localFolder.Path);
                _rhoruntime.setCryptoEngine(new CryptoEngineWrapper(new RhoCrypt()));
                _rhoruntime.setOsVersion(major, minor, build);
                // create and start rhodes main thread               
                _rhoruntimeThread = Task.Run(() => {
                    _rhoruntime.Execute();
                    deb("rhoruntime thread loop closed"); });

                //temporary solutions, to do refactoring
                DoWait(200);
                _rhoruntime.onActivate(0);
            }
            catch (Exception e)
            {
                RhodesWebBrowser.NavigateToString("<html><head><title>Exception</title></head><body>Exception: " + e.Message + "</body></html>");
                deb("InitializeExtensions exceptions " + e.Message);
            }
            deb("User agent: " + _userAgent);
            deb("Constructor ended");
        }

        //[System.Runtime.CompilerServices.MethodImpl(System.Runtime.CompilerServices.MethodImplOptions.NoInlining)]
        public static void deb(String s)
        {
            System.Diagnostics.Debug.WriteLine(s);
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
            return "IE/" + DEVICE_INFO.OperatingSystem + "." +
                DEVICE_INFO.SystemFirmwareVersion;
        }

        public string getScreenOrientation()
        {
            //SensorReadingType
            if (_screenOrientation == SimpleOrientation.Facedown ||
                _screenOrientation == SimpleOrientation.Faceup ||
                _screenOrientation == SimpleOrientation.NotRotated ||
                _screenOrientation == SimpleOrientation.Rotated180DegreesCounterclockwise)
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


        public static Task InvokeInUIThread(Action action)
        {
            var tcs = new TaskCompletionSource<bool>();
            if (dispatcher == null) {
                action();
                tcs.TrySetResult(true);
                return tcs.Task;
            }
            var ignore = dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>{
                try{
                    action();
                    tcs.TrySetResult(true);
                }
                catch (Exception ex){
                    tcs.TrySetException(ex);
                    deb("Invoke in UI Thread exception");
                }
            });
            return tcs.Task;
        }


        public void bringToFront()
        {
           
            if (!isUIThread) { InvokeInUIThread(delegate() { bringToFront(); }); return; }
            deb("Bringing to front");
            this.bringToFront();
        }

        public void performOnUiThread(Int64 native)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { performOnUiThread(native); }); return; }
            deb("Performing on ui Thread");
            CRhoRuntime.getInstance().executeRhoRunnable(native);
        }

        private void PhoneApplicationPage_BackKeyPress(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //e.Cancel = CRhoRuntime.getInstance().onBackKeyPress();
        }

        private void updateOrientation(SimpleOrientation orientation)
        {
            deb("Updating orientation");
            _screenOrientation = orientation;
            updateScreenSize();
        }

        private void updateScreenSize()
        {
            if (!isUIThread) { InvokeInUIThread(delegate () { updateScreenSize(); }); return; }
            deb("Updating screen size");
            var bounds = ApplicationView.GetForCurrentView().VisibleBounds;
            var scaleFactor = Windows.Graphics.Display.DisplayInformation.GetForCurrentView().RawPixelsPerViewPixel;
            var size = new Size(bounds.Width * scaleFactor, bounds.Height * scaleFactor);

            var dpi = Windows.Graphics.Display.DisplayInformation.GetForCurrentView().RawDpiY;

            _screenWidth = size.Width;
            _screenHeight = size.Height;
            _screenPhysicalHeight = size.Width/dpi; // assuming 4 inches
            _screenPhysicalWidth = size.Height/dpi; // assuming square pixels

            try { _screenOrientation = SimpleOrientationSensor.GetDefault().GetCurrentOrientation(); }
            catch {
                _screenOrientation = SimpleOrientation.Faceup;
                deb("Screen orientation exception");
            }
            if ((_screenOrientation == SimpleOrientation.Rotated270DegreesCounterclockwise) || (_screenOrientation == SimpleOrientation.Rotated90DegreesCounterclockwise))
            {
                double w = _screenWidth;
                _screenWidth = _screenHeight;
                _screenHeight = w;
                w = _screenPhysicalWidth;
                _screenPhysicalWidth = _screenPhysicalHeight;
                _screenPhysicalHeight = w;
            }
        }

        private void ApplicationPage_OrientationChanged(object sender, SimpleOrientationSensorOrientationChangedEventArgs e)
        {
            updateOrientation(e.Orientation);
        }

        private void ApplicationPage_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            updateScreenSize();
        }

        private void ApplicationPage_Loaded(object sender, RoutedEventArgs e)
        {   
            var browser = new WebView();
            browser.Settings.IsJavaScriptEnabled = true;
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
            if (!isUIThread) { InvokeInUIThread(delegate() { navigate(url, index); }); return; }
            /*if(url.Contains("http://") == false)
            {
                String appdir = Windows.Storage.ApplicationData.Current.LocalFolder.Path;
                url = "file:///" + appdir.Replace(@"\", "/") +"/" + url;
               
            }*/

            if (_tabProps.Count == 0) index = -1;
            deb("Navigated to " + url);
            if (url == "") return;

            if (index == -1 && !_isBrowserInitialized)
            {
                initUri = url;
                deb("Browser not Initialized");
                return;
            }
            else if (index > -1 && _tabProps.ContainsKey(index) && _tabProps[index]._isInitialized == false)
            {
                _tabProps[index]._action = url;
                return;
            }

            if (TabbarPivot.Items.Count == 0)
                navigateWebViewToLink(RhodesWebBrowser, url);
            else
            {
                WebView wv = (WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content;
                navigateWebViewToLink(wv, url);
            }

        }

        public string executeScriptFunc(string script, int index)
        {
            string[] codeString = { script };
            if (TabbarPivot.Items.Count == 0 || _isCallbackFired)//_tabProps[index]._isInitialized == false)
            {
                _isCallbackFired = false;

                var task = RhodesWebBrowser.InvokeScriptAsync("eval", codeString);
                task.AsTask().Wait();
                return task.GetResults();
            }
            else {
                var task = ((WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).InvokeScriptAsync("eval", codeString);
                task.AsTask().Wait();
                return task.GetResults();
            }
        }

        public string executeScript(string script, int index)
        {
            deb("executeScript: " + script);
            return StringValueByStringIntReturnAgent(executeScriptFunc, script, index);
        }

        public string executeScriptAsync(string script, int index)
        {
            deb("executeScriptAsync: " + script);
            InvokeInUIThread(() =>
            {
                string[] codeString = { script };
                if (TabbarPivot.Items.Count == 0 || _isCallbackFired)
                {
                    _isCallbackFired = false;
                    var task = RhodesWebBrowser.InvokeScriptAsync("eval", codeString).Completed = (arg1, arg2) => { deb("Async task executeon complited"); };
                    deb("Creating AsyncTask ends");
                }
                else
                {
                    var task = ((WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).InvokeScriptAsync("eval", codeString);
                }
            });

            return "";
            //return StringValueByStringIntReturnAgentAsync(executeScriptFunc, script, index);
        }

        public void GoBack()
        {
            GoBack(TabbarPivot.SelectedIndex);
        }

        public void GoBack(int index)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { GoBack(index); }); return; }
            deb("Going back");
            if ((TabbarPivot.Items.Count == 0)/* || (index < 0) || (index >= TabbarPivot.Items.Count)*/)
            {
                if (RhodesWebBrowser.CanGoBack)
                    RhodesWebBrowser.GoBack();
            }
            else
            {
                if (((WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).CanGoBack)
                    ((WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).GoBack();
            }
        }

        public void GoForward()
        {
            GoForward(TabbarPivot.SelectedIndex);
        }

        public void GoForward(int index)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { GoForward(index); }); return; }
            deb("Going forward");
            if ((TabbarPivot.Items.Count == 0) || (index < 0) || (index >= TabbarPivot.Items.Count))
            {
                if (RhodesWebBrowser.CanGoForward)
                    RhodesWebBrowser.GoForward();
            }
            else
            {
                if (((WebView)((PivotItem)TabbarPivot.Items[index]).Content).CanGoForward)
                    ((WebView)((PivotItem)TabbarPivot.Items[index]).Content).GoForward();
            }
        }

        public void Refresh(int index)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { Refresh(index); }); return; }
            deb("Refreshing");
            if (TabbarPivot.Items.Count == 0)
                navigateWebViewToLink(RhodesWebBrowser, RhodesWebBrowser.Source.AbsoluteUri);
            // another possible implementation: RhodesWebBrowser.InvokeScript("eval", "history.go()");
            else
            {
                WebView wv = (WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content;
                navigateWebViewToLink(wv, wv.Source.AbsoluteUri);
            }                
        }

        public void navigateWebViewToLink(WebView currentBrowser, String link)
        {
            //currentBrowser.Settings.IsJavaScriptEnabled = true;
            //currentBrowser.NavigateToLocalStreamUri(currentBrowser.BuildLocalStreamUri("Content", link), new UriToStreamResolver());
            //currentBrowser.NavigateToLocalStreamUri(new Uri(link, UriKind.Absolute), uriResolver);
            
            currentBrowser.Navigate(new Uri(link, UriKind.Absolute));
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
                    return ((WebView)((PivotItem)TabbarPivot.Items[getValidTabbarIndex(index)]).Content).Source.ToString();
            }
            catch (Exception)
            {
                deb("getCurrentURLFunc exception");
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

        private void RhodesWebBrowser_Navigated(object sender, WebViewNavigationCompletedEventArgs e)
        {
            String url = getCurrentURLFunc(((WebViewTag)((sender as WebView).Tag)).getTabIndex());
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
        }

        private void RhodesWebBrowser_NavigationFailed(object sender, WebViewNavigationFailedEventArgs e)
        {
            // TODO: WebView NavigationFailed - do we need this?
        }

        private void RhodesWebBrowser_LoadCompleted(object sender, /*WebViewContentLoadingEventArgs*/NavigationEventArgs e)
        {
            String url = getCurrentURLFunc(((WebViewTag)((sender as WebView).Tag)).getTabIndex());
            CRhoRuntime.getInstance().onWebViewUrlChanged(url);
            WebView wView = (sender as WebView);
            int index = ((WebViewTag)(wView.Tag)).getTabIndex();
            //(asender, args) => browser.NavigateToString(getUserAgentHtml);
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
            try { CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(((WebViewTag)((sender as WebView).Tag)).getTabIndex())); }
            catch { 
                deb("RhodesWebBrowser_Loaded exception");
                try{
                    deb("Trying to load getCurrentURLFunc(0) exception");
                    CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(0));
                }
                catch
                {
                    deb("onWebViewUrlChanged(getCurrentURLFunc(0)) exception");
                }
            }
        }


        private void RhodesWebBrowser_Unloaded(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onWebViewUrlChanged(getCurrentURLFunc(((WebViewTag)((sender as WebView).Tag)).getTabIndex()));
        }

        private void RhodesWebBrowser_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            // TODO: webview size changed event handler ?
        }



        private void updateAppBarModeAndVisibility(bool autoToolbarVisibility = true)
        {
            if (!isUIThread) {
                InvokeInUIThread(delegate () {updateAppBarModeAndVisibility(autoToolbarVisibility); });
                return;
            }
            if (autoToolbarVisibility) { deb("Tool bar visible"); }
            else { deb("Tool bar invisible"); }
            //getAppBar().Visibility = autoToolbarVisibility ? Visibility.Visible : Visibility.Collapsed;
            getAppBar().Visibility = Visibility.Visible;
            //getAppBar().IsSticky = autoToolbarVisibility ? 
            if (autoToolbarVisibility && (getAppBar().PrimaryCommands.Count > 0))
            {
                getAppBar().ClosedDisplayMode = AppBarClosedDisplayMode.Compact;
                getAppBar().IsOpen = true;
                //ApplicationBar.IsMenuEnabled = true;
                getAppBar().Opacity = 1.0;
                deb("Tool bar type 1");
            }
            else if (getAppBar().PrimaryCommands.Count > 0)
            {
                getAppBar().ClosedDisplayMode = AppBarClosedDisplayMode.Minimal;
                getAppBar().IsOpen = true;
                //ApplicationBar.IsMenuEnabled = true;
                getAppBar().Opacity = 0.75;
                deb("Tool bar type 2");
            }
            else
            {
                getAppBar().ClosedDisplayMode = AppBarClosedDisplayMode.Compact;
                getAppBar().IsOpen = false;
                //ApplicationBar.IsMenuEnabled = false;
                getAppBar().Opacity = 1.0;
                deb("Tool bar type 3");
            }
        }

        // *** TOOLBAR ***

        public void toolbarRemoveAllButtons()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { toolbarRemoveAllButtons(); }); return; }
            deb("Removing buttons");
            foreach(ICommandBarElement value in getAppBar().PrimaryCommands.ToList()){
                if (value.GetType() == typeof(AppBarButton)){
                    getAppBar().PrimaryCommands.Remove(value);
                }
            }
            
            toolbarItems.Clear();

            // remove all toolbar items added to menu (uwp toolbar allows 4 buttons only)
            int j = toolbarMenuItems.Count - 1;
            if (j >= 0){
                for (int i = getAppBar().SecondaryCommands.Count - 1; i >= 0; i--){
                    if (getAppBar().SecondaryCommands.ToList()[i].Equals(toolbarMenuItems[j])){
                        getAppBar().SecondaryCommands.RemoveAt(i);
                        if ((--j) < 0) break;
                    }
                }
            }
            toolbarMenuItems.Clear();
            updateAppBarModeAndVisibility();
            getAppBar().Visibility = Visibility.Collapsed;
        }

        public void toolbarShow()
        {
            
            if (!isUIThread) { InvokeInUIThread(delegate() { toolbarShow(); }); return; }
            deb("Showing tabbar");
            updateAppBarModeAndVisibility();
            getAppBar().Visibility = Visibility.Visible;
        }

        public void toolbarHide()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { toolbarHide(); }); return; }
            deb("Hiding tabbar");
            updateAppBarModeAndVisibility(false);
            getAppBar().Visibility = Visibility.Collapsed;
        }

        private int toolbarIsStartedFunc()
        {
            foreach (ICommandBarElement value in getAppBar().PrimaryCommands.ToList())
            {
                if (value.GetType() == typeof(AppBarButton))
                {
                    return 1;
                }
            }
            return 0;
        }

        public int toolbarIsStarted()
        {
            return IntValueReturnAgent(toolbarIsStartedFunc);
        }

        private int toolbarGetHeightFunc()
        {
            return getAppBar().Visibility == Windows.UI.Xaml.Visibility.Visible? (int)getAppBar().Height : 0;
        }

        public int toolbarGetHeight()
        {
            return IntValueReturnAgent(toolbarGetHeightFunc);
        }

        public void toolbarAddAction(string text)
        {
            getAppBar().Visibility = Visibility.Visible;
            toolbarAddAction(null, text, text);
        }

        public void toolbarAddAction(string icon, string text, string action)
        {
            if (!isUIThread) { InvokeInUIThread(delegate () { toolbarAddAction(icon, text, action); }); return; }
            deb("Adding action to toolbar: " + action + " ; icon: " + icon);
            getAppBar().Visibility = Visibility.Visible;
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
            if (!isUIThread) { InvokeInUIThread(delegate() { toolbarAddAction(icon, text, action); }); return; }

            int buttonsCount = 0;
            foreach (ICommandBarElement value in getAppBar().PrimaryCommands.ToList()) {
                if (value.GetType() == typeof(AppBarButton)) buttonsCount++;    
            }
            if (buttonsCount < 4)
            {
                
                AppBarButton toolbarButton = new AppBarButton();
                BitmapIcon bitmapIcon = new BitmapIcon();
                try
                {
                    //Uri iconUri = new Uri((icon != null) && (icon.Length > 0) ? prependWithSlash(icon) : "/rho/apps/public/images/cancel.png", UriKind.Absolute);
                    Uri iconUri = null;
                    if ((icon != null) && (icon.Length > 0)){iconUri = new Uri(icon, UriKind.Absolute);}
                    else{iconUri = new Uri("/rho/apps/public/images/cancel.png", UriKind.Relative);}
                    bitmapIcon.UriSource = iconUri;
                }catch(Exception e){
                    deb("Add ToolBar exception: " + e.Message);
                }
                toolbarButton.Icon = bitmapIcon;
                //deb("Real icon Uri: " + bitmapIcon.UriSource.AbsolutePath);
                toolbarButton.Label = text;
                getAppBar().PrimaryCommands.Add(toolbarButton);
                toolbarButton.Click += new RoutedEventHandler(toolbarButton_Click);
            }
            else
            {
                AppBarButton toolbarMenuButton = new AppBarButton();
                toolbarMenuButton.Label = text;
                getAppBar().SecondaryCommands.Add(toolbarMenuButton);
                toolbarMenuButton.Click += new RoutedEventHandler(toolbarButton_Click);
                toolbarMenuItems.Add(toolbarMenuButton);
            }
            toolbarItems.Add(text, action);
            updateAppBarModeAndVisibility();
        }
        
        public void toolbarAddSeparator()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { toolbarAddSeparator(); }); return; }
            deb("Addinng separator to toolbar");
            CRhoRuntime.getInstance().logEvent("Toolbar separator is unimplemented on UWP");
        }

        public void setToolbarStyle(bool border, string background, string mask)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { setToolbarStyle(border, background, mask); }); return; }
            deb("Setting toobar style");
            // TODO: implement setToolbarStyle
            if (background != "") getAppBar().Background = new SolidColorBrush(getColorFromString(background));
            // TODO: implement opacity for pictures
        }

        private void toolbarButton_Click(object sender, RoutedEventArgs e)
        {
            CRhoRuntime.getInstance().onToolbarAction(toolbarItems[(sender as AppBarButton).Label]);
        }

        /*private void toolbarMenuItem_Click(object sender, EventArgs e)
        {
            CRhoRuntime.getInstance().onToolbarAction(toolbarItems[(sender as AppBarButton).Text]);
        }*/


        // *** MENU ***

        public void menuClear()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { menuClear(); }); return; }
            deb("Cleaning menu");
            getAppBar().SecondaryCommands.Clear();
            // restore menu items converted from toolbar buttons
            toolbarMenuItems.ForEach(item => getAppBar().SecondaryCommands.Remove(item));
            menuItems.Clear();
            updateAppBarModeAndVisibility();
            getAppBar().Visibility = Visibility.Collapsed;
        }

        public void menuAddAction(string label, int item)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { menuAddAction(label, item); }); return; }
            deb("Adding action to menu " + label);
            getAppBar().Visibility = Visibility.Visible;
            AppBarButton menuItem = new AppBarButton();
            menuItem.Label = label;
            getAppBar().SecondaryCommands.Add(menuItem);
            menuItem.Click += (e,f) => { menuItem_Click(item); };
            updateAppBarModeAndVisibility();
        }

        public void menuAddSeparator()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { menuAddSeparator(); }); return; }
            deb("Adding separator to menu");
            CRhoRuntime.getInstance().logEvent("Menu separator is unimplemented on UWP");
        }

        private void menuItem_Click(int index)
        {
            CRhoRuntime.getInstance().onCustomMenuItemCommand(index);
        }


        // *** TABBAR ***

        public void tabbarInitialize()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { tabbarInitialize(); }); return; }
            deb("Initializing tabbar");
            tabbarRemoveAllTabs();
            // TODO: clear style of tabBar!
        }

        public void tabbarRemoveAllTabs()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { tabbarRemoveAllTabs(); }); return; }
            deb("Removing tabs from tabbar");
            TabbarPivot.Items.Clear();
            _tabProps.Clear();
            _oTabResult = null;
        }

        public void tabbarRemove(int index)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { tabbarRemove(index); }); return; }
            deb("Removing tab " + index + " from tabbar");
            TabbarPivot.Items.RemoveAt(index);
        }

        public void tabbarShow()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { tabbarShow(); }); return; }
            deb("Showing tabbar");
            LayoutRoot.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Star);
            LayoutRoot.RowDefinitions[1].Height = new GridLength(0, GridUnitType.Pixel);
            // not needed: TabbarPanel.Visibility = System.Windows.Visibility.Visible;
        }

        public void tabbarHide()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { tabbarHide(); }); return; }
            deb("Hiding tabbar");
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
            return TabbarPivot.Items.Count > 0 ? (int)(TabbarPivot.ActualHeight - ((WebView)TabbarPivot.SelectedItem).ActualHeight) : 0;
        }

        public int tabbarGetHeight()
        {
            return IntValueReturnAgent(tabbarGetHeightFunc);
        }

        public void tabbarSwitch(int index)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { tabbarSwitch(index); }); return; }
            deb("Switching tabbar");
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

        private static Windows.UI.Color getColorFromString(string strColor)
        {
            if (strColor == null || strColor == "")
                return Windows.UI.Color.FromArgb(255, 0, 0, 0);

            int c = Convert.ToInt32(strColor);

            int cR = (c & 0xFF0000) >> 16;
            int cG = (c & 0xFF00) >> 8;
            int cB = (c & 0xFF);

            return Windows.UI.Color.FromArgb(255, Convert.ToByte(cR), Convert.ToByte(cG), Convert.ToByte(cB));
        }

        public void tabbarAddTab(string label, string icon, string action, bool disabled, string web_bkg_color, string selected_color, string background_color, bool reload, bool use_current_view_for_tab, bool hasCallback, IMethodResult oResult)
        {
            if (!isUIThread) { InvokeInUIThread(delegate () { tabbarAddTab(label, icon, action, disabled, web_bkg_color, selected_color, background_color, reload, use_current_view_for_tab, hasCallback, oResult); }); return; }
            deb("Adding Tab to tabbar");
            PivotItem tab = new PivotItem();
            TabHeader th = new TabHeader();

            th.Label = label;
            th.Background = th.UnselectedBackground = new SolidColorBrush(
                (background_color != null) && (background_color.Length > 0) ?
                getColorFromString(background_color) :
                Windows.UI.Color.FromArgb(255, 0, 0, 0)
                );
            th.SelectedBackground = (selected_color != null) && (selected_color.Length > 0) ?
                new SolidColorBrush(getColorFromString(selected_color)) :
                th.UnselectedBackground;
            try
            {               
                th.IconImage = (icon == null) || (icon.Length == 0) ?
                    new Windows.UI.Xaml.Media.Imaging.BitmapImage() : 
                    new Windows.UI.Xaml.Media.Imaging.BitmapImage(new Uri(prependWithSlash(icon), UriKind.Relative));
            }catch(Exception e)
            {
                th.IconImage = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
                deb("TabHeader creater: " +  e.Message);
            }
            
            tab.Header = th;

            _tabProps[TabbarPivot.Items.Count] = new TabProps();
            _tabProps[TabbarPivot.Items.Count]._isReload = reload;
            if (hasCallback == true) _oTabResult = oResult;

            Windows.UI.Xaml.Controls.WebView wv = new Windows.UI.Xaml.Controls.WebView();
            wv.Height = double.NaN;
            wv.Width = double.NaN;
            wv.SizeChanged += RhodesWebBrowser_SizeChanged;
            wv.NavigationCompleted += RhodesWebBrowser_Navigated;
            wv.NavigationFailed += RhodesWebBrowser_NavigationFailed;
            wv.LoadCompleted += RhodesWebBrowser_LoadCompleted;
            wv.Loaded += RhodesWebBrowser_Loaded;
            wv.Unloaded += RhodesWebBrowser_Unloaded;
            wv.NavigationStarting += webViewNavigationStarting;

            wv.Source = new Uri("about:blank");
            wv.Tag = new WebViewTag(TabbarPivot.Items.Count);
            wv.Settings.IsJavaScriptEnabled = true;
            wv.Settings.IsIndexedDBEnabled = true;

            if ((web_bkg_color != null) && (web_bkg_color.Length > 0)) wv.DefaultBackgroundColor = getColorFromString(web_bkg_color);
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

        private void TabbarPivot_SelectionChanged(object sender, SelectionChangedEventArgs e)
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
            CRhoRuntime.getInstance().logEvent("Tab badge is not implemented on universal windows platform");
        }


        // *** COMMANDS ***

        // this method is an example of application exit technique (see Quit method in App.xaml.cs)
        public void exitCommand()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { DoWait(100); exitCommand(); }); return; }
            deb("Exit comand");
            var propertySet = Windows.Storage.ApplicationData.Current.LocalSettings.Values;
            Application.Current.Exit();
        }

        public void navigateBackCommand()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { navigateBackCommand(); }); return; }
            deb("Go back");
            this.GoBack();
        }

        public void navigateForwardCommand()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { navigateForwardCommand(); }); return; }
            deb("Navigating forward command");
            this.GoForward();
        }

        public void logCommand()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { logCommand(); }); return; }
            deb("Logging command");
        }

        public void refreshCommand(int tab_index)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { refreshCommand(tab_index); }); return; }
            deb("Refreshing command");
            this.Refresh(tab_index);
        }

        public void takePicture(string callbackUrl)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { takePicture(callbackUrl); }); return; }
        }

        public void selectPicture(string callbackUrl)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { selectPicture(callbackUrl); }); return; }
        }

        public void alertHidePopup()
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { alertHidePopup(); }); return; }
        }

        //public void dateTimePicker(CDateTimeMessage *) {}

        //public void takeSignature(void*); // Signature::Params*

        /*private int isFullscreenFunc()
        {

            return SystemTray.IsVisible ? 0 : 1;
        }*/
        private void webViewNavigationStarting(WebView sender, WebViewNavigationStartingEventArgs args)
        {
            sender.Settings.IsJavaScriptEnabled = true;
            sender.AddWebAllowedObject("rhoRuntimeInvoker", new RuntimeInvoker());
        }

        public bool isFullscreen()
        {
            return ApplicationView.GetForCurrentView().IsFullScreenMode;
        }

        public void fullscreenCommand(int fullScreen)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { fullscreenCommand(fullScreen); }); return; }
            deb("Fullscreen command");
            if (fullScreen != 0) {
                ApplicationView.GetForCurrentView().TryEnterFullScreenMode();
                //var statusBar = StatusBar.GetForCurrentView();
                //await statusBar.HideAsync();
            }
            else { ApplicationView.GetForCurrentView().ExitFullScreenMode(); }
        }

        public void setCookie(string url, string cookie)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { setCookie(url, cookie); }); return; }
            // TODO: implement
        }


        // *** MISC ***
        // this method is used as a callback for calling the C# API methods from C++
        public void DoWait(int timeout)
        {
            Task task = Task.Delay(TimeSpan.FromMilliseconds(timeout));
            task.Wait();            
        }

        public void DisplayMessage(string msg)
        {
            if (!isUIThread) { InvokeInUIThread(delegate() { DisplayMessage(msg); }); return; }
            deb("Displaying message");
            RhodesWebBrowser.NavigateToString("<html><head><title>Message</title></head><body>" + msg + "</body></html>");
        }

        private int IntValueReturnAgent(Func<int> func)
        {
            if (isUIThread) return func();

            Exception exception = null;
            var waitEvent = new System.Threading.ManualResetEvent(false);
            int return_value = 0;
            InvokeInUIThread(() =>{
                try{return_value = func();}
                catch (Exception ex){
                    exception = ex;
                    deb(ex.Message);
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
            InvokeInUIThread(() =>
            {
                try
                {
                    return_value = func(index);
                }
                catch (Exception ex)
                {
                    exception = ex;
                    deb(ex.Message);
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
            if (isUIThread) return func(str, index);

            Exception exception = null;
            var waitEvent = new System.Threading.ManualResetEvent(false);
            string return_value = "";
            InvokeInUIThread(() =>
            {
                try
                {
                    return_value = func(str, index);
                }
                catch (Exception ex)
                {
                    exception = ex;
                    deb(ex.Message);
                }
                waitEvent.Set();
            });

             //waitEvent.WaitOne();
            if (exception != null) throw exception;
            return return_value;
        }

        private string StringValueByStringIntReturnAgentAsync(Func<string, int, string> func, string str, int index)
        {
            if (isUIThread) return func(str, index);

            Exception exception = null;
            string return_value = "";
            InvokeInUIThread(() =>
            {
                try
                {
                    return_value = func(str, index);
                }
                catch (Exception ex)
                {
                    exception = ex;
                    deb(ex.Message);
                }
            });

            if (exception != null)
                throw exception;

            return return_value;
        }

/*        private void RhodesWebBrowser_JSNotify(object sender, NotifyEventArgs e)
        {
            deb("Invoking JS: " + e.Value);
            String answer = CRhoRuntime.getInstance().onJSInvoke(e.Value);
            Windows.UI.Xaml.Controls.WebView wb = sender as Windows.UI.Xaml.Controls.WebView;
            wb.Settings.IsJavaScriptEnabled = true;
            
            var result = wb.InvokeScriptAsync("__rhoNativeApiResult", new string[] { answer });
            result.AsTask().Wait();
        }*/
        
    }
}
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409
/*
namespace rhodes
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }
    }
}*/

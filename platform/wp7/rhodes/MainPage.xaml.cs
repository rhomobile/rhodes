using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using System.Xml.Linq;


using System.IO.IsolatedStorage;
using System.IO;
using System.Windows.Resources;
using rho.common;
   
namespace Rhodes
{
    public partial class MainPage : PhoneApplicationPage
    {
        private static string GetWinPhoneAttribute(string attributeName)
        {
            string ret = string.Empty;

            try
            {
                XElement xe = XElement.Load("WMAppManifest.xml");
                var attr = (from manifest in xe.Descendants("App")
                            select manifest).SingleOrDefault();
                if (attr != null)
                    ret = attr.Attribute(attributeName).Value;
            }
            catch
            {
                // Ignore errors in case this method is called
                // from design time in VS.NET
            }

            return ret;
        }
        
        // Constructor
        public MainPage()
        {
            InitializeComponent();
            ApplicationTitle.Text = GetWinPhoneAttribute("Title");

            webBrowser1.Visibility = Visibility.Collapsed;
            webBrowser1.IsScriptEnabled = true;

            webBrowser1.Loaded += WebBrowser_OnLoaded;
            webBrowser1.LoadCompleted += WebBrowser_OnLoadCompleted;
            webBrowser1.Navigating += WebBrowser_OnNavigating;
            webBrowser1.Navigated += WebBrowser_OnNavigated;
            webBrowser1.ScriptNotify += WebBrowser_OnScriptNotify;
        }

        private CRhodesApp RHODESAPP(){return CRhodesApp.Instance;}

        //private void Navigate(string url)
        //{
            //webBrowser1.NavigateToString("Hello!!!");
        //}

        //window.external.notify(<data>)
        private void WebBrowser_OnScriptNotify(object sender, NotifyEventArgs e)
        {
            //webBrowser1.InvokeScript("test_call3", "one");
        }

        private void WebBrowser_OnLoaded(object sender, RoutedEventArgs e)
        {
            RHODESAPP().Init(webBrowser1, this);
        }

        private void WebBrowser_OnLoadCompleted(object sender, NavigationEventArgs e)
        {
        }

        private void WebBrowser_OnNavigating(object sender, NavigatingEventArgs e)
        {
            if (!RHODESAPP().HttpServer.processBrowserRequest(e.Uri))
                return;

            e.Cancel = true;
        }

        private void WebBrowser_OnNavigated(object sender, NavigationEventArgs e)
        {
            if (webBrowser1.Visibility == Visibility.Collapsed)
            {
                LoadingImage.Visibility = Visibility.Collapsed;
                webBrowser1.Visibility = Visibility.Visible;
            }
            RHODESAPP().addToHistory(e.Uri);
        }

    }
}
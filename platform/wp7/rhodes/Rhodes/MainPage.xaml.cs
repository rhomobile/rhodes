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

using System.IO.IsolatedStorage;
using System.IO;
using System.Windows.Resources;

namespace Rhodes
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();

            webBrowser1.IsScriptEnabled = true;

            webBrowser1.Loaded += WebBrowser_OnLoaded;
            webBrowser1.LoadCompleted += WebBrowser_OnLoadCompleted;
            webBrowser1.Navigating += WebBrowser_OnNavigating;
            webBrowser1.Navigated += WebBrowser_OnNavigated;
            webBrowser1.ScriptNotify += WebBrowser_OnScriptNotify;
        }

        //window.external.notify(<data>)
        private void WebBrowser_OnScriptNotify(object sender, NotifyEventArgs e)
        {
            webBrowser1.InvokeScript("test_call3", "one");
        }

        private void WebBrowser_OnLoaded(object sender, RoutedEventArgs e)
        {
            //SaveFilesToIsoStore();
            //webBrowser1.Navigate(new Uri("http://www.google.com", UriKind.Absolute));
            SaveFilesToIsoStore();

            webBrowser1.IsScriptEnabled = true;
            webBrowser1.Navigate(new Uri("readme.htm", UriKind.Relative));
        }

        private void WebBrowser_OnLoadCompleted(object sender, NavigationEventArgs e)
        {
        }

        private void WebBrowser_OnNavigating(object sender, NavigatingEventArgs e)
        {
            /* if (e.Uri.AbsoluteUri.Equals("http://www.google.com/") )
             {
                 e.Cancel = true;
                 webBrowser1.Navigate(new Uri("http://www.yahoo.com", UriKind.Absolute));
             }*/

            //string strPage = webBrowser1.SaveToString();
            //if (strPage != null)
            //    return;
        }

        private void WebBrowser_OnNavigated(object sender, NavigationEventArgs e)
        {
        }

        private void SaveFilesToIsoStore()
        {
            //These files must match what is included in the application package,
            //or BinaryStream.Dispose below will throw an exception.
            string[] files = {
            "readme.htm",
            "windows_mobile.css"
            //"test.jpg"
        };

            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();

            if (false == isoStore.FileExists(files[0]))
            {
                foreach (string f in files)
                {
                    StreamResourceInfo sr = Application.GetResourceStream(new Uri(f, UriKind.Relative));
                    using (BinaryReader br = new BinaryReader(sr.Stream))
                    {
                        byte[] data = br.ReadBytes((int)sr.Stream.Length);
                        SaveToIsoStore(f, data);
                    }
                }
            }
        }

        private void SaveToIsoStore(string fileName, byte[] data)
        {
            string strBaseDir = string.Empty;
            string delimStr = "/";
            char[] delimiter = delimStr.ToCharArray();
            string[] dirsPath = fileName.Split(delimiter);

            //Get the IsoStore
            IsolatedStorageFile isoStore = IsolatedStorageFile.GetUserStoreForApplication();

            //Recreate the directory structure
            for (int i = 0; i < dirsPath.Length - 1; i++)
            {
                strBaseDir = System.IO.Path.Combine(strBaseDir, dirsPath[i]);
                isoStore.CreateDirectory(strBaseDir);
            }

            //Remove existing file
            if (isoStore.FileExists(fileName))
            {
                isoStore.DeleteFile(fileName);
            }

            //Write the file
            using (BinaryWriter bw = new BinaryWriter(isoStore.CreateFile(fileName)))
            {
                bw.Write(data);
                bw.Close();
            }
        }

    }
}
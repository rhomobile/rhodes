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
using System.Linq;
using System.Net;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using rho.common;
using Windows.Storage;
using System.IO.IsolatedStorage;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Threading;

namespace rhodes
{
    class AssociationUriMapper : UriMapperBase
    {
        private string tempUri;

        public override Uri MapUri(Uri uri)
        {
            tempUri = System.Net.HttpUtility.UrlDecode(uri.ToString());

            // URI association launch for contoso.
            if (tempUri.Contains("symats:LaunchTestApplication"))
            {
                Debug.WriteLine("Launching with URI: " + tempUri);
                // Get the RhoConfig.Txt file.
                int rhoConfigTxtIndex = tempUri.IndexOf("RhoConfigTxt=") + 13;
                string rhoConfigUrl = tempUri.Substring(rhoConfigTxtIndex);

                //  Replace the start_path in the default config with our start path
                var success = Task.Run(() => ModifyConfiguration(rhoConfigUrl)).GetAwaiter().GetResult();

                // Map the show products request to ShowProducts.xaml
                return new Uri("/MainPage.xaml", UriKind.Relative);
            }

            // Otherwise perform normal launch.
            return uri;
        }


        public async Task<bool> ModifyConfiguration(string rhoConfigUrl)
        {
            bool bSuccess = false;
            StorageFolder local = Windows.Storage.ApplicationData.Current.LocalFolder;
            if (local != null)
            {
                var dataFolder = await local.GetFolderAsync("rho");
                var rhoFolder = await dataFolder.GetFolderAsync("apps");
                var file = await rhoFolder.OpenStreamForReadAsync("rhoconfig.txt");
                string configFileAsString = null;
                using (StreamReader streamReader = new StreamReader(file))
                {
                    configFileAsString = streamReader.ReadToEnd();
                }
                file.Close();

                if (configFileAsString != null)
                {

                    //  Modify the config here
                    int startPageIndex = configFileAsString.IndexOf("start_path");
                    if (startPageIndex > -1)
                    {
                        int startPageIndexEnd = (configFileAsString.Substring(startPageIndex).IndexOf("\r\n"));
                        if (startPageIndexEnd > -1)
                        {
                            startPageIndexEnd += startPageIndex;
                            string newStartPath = "start_path = '" + rhoConfigUrl + "'";
                            string beforeStartPath = configFileAsString.Substring(0, startPageIndex);
                            string afterStartPath = configFileAsString.Substring(startPageIndexEnd, (configFileAsString.Length - startPageIndexEnd));
                            string newConfigFile = beforeStartPath + newStartPath + afterStartPath;
                            //Debug.WriteLine("New Config: " + newConfigFile);
                            byte[] fileBytes = System.Text.Encoding.UTF8.GetBytes(newConfigFile);
                            var writeFile = await rhoFolder.CreateFileAsync("rhoconfig.txt", CreationCollisionOption.ReplaceExisting);
                            using (var s = await writeFile.OpenStreamForWriteAsync())
                            {
                                s.Write(fileBytes, 0, fileBytes.Length);
                            }
                            bSuccess = true;
                        }
                    }
                }
            }
            return bSuccess;
        }
    }

	
    public partial class App : Application
    {
        // exception to exit application
        private class QuitException : Exception { }

        /// <summary>
        /// Provides easy access to the root frame of the Phone Application.
        /// </summary>
        /// <returns>The root frame of the Phone Application.</returns>
        public PhoneApplicationFrame RootFrame { get; private set; }

        /// <summary>
        /// Constructor for the Application object.
        /// </summary>
        public App()
        {
            // Global handler for uncaught exceptions. 
            UnhandledException += Application_UnhandledException;

            // Standard Silverlight initialization
            InitializeComponent();

            // Phone-specific initialization
            InitializePhoneApplication();

            // Show graphics profiling information while debugging.
            if (System.Diagnostics.Debugger.IsAttached)
            {
                // Display the current frame rate counters.
                Application.Current.Host.Settings.EnableFrameRateCounter = true;

                // Show the areas of the app that are being redrawn in each frame.
                //Application.Current.Host.Settings.EnableRedrawRegions = true;

                // Enable non-production analysis visualization mode, 
                // which shows areas of a page that are handed off to GPU with a colored overlay.
                //Application.Current.Host.Settings.EnableCacheVisualization = true;

                // Disable the application idle detection by setting the UserIdleDetectionMode property of the
                // application's PhoneApplicationService object to Disabled.
                // Caution:- Use this under debug mode only. Application that disables user idle detection will continue to run
                // and consume battery power when the user is not using the phone.
                PhoneApplicationService.Current.UserIdleDetectionMode = IdleDetectionMode.Disabled;
            }

        }

        // throw unhandled exception to exit application (see Application_UnhandledException below)
        public static void Quit()
        {
            throw new QuitException();
        }

        public void CopyResourceFiles()
        {

        }

        // Code to execute when the application is launching (eg, from Start)
        // This code will not execute when the application is reactivated
        private void Application_Launching(object sender, LaunchingEventArgs e)
        {
            // TODO: add app init code here ?
        }

        // Code to execute when the application is activated (brought to foreground)
        // This code will not execute when the application is first launched
        private void Application_Activated(object sender, ActivatedEventArgs e)
        {
            rhoruntime.CRhoRuntime.getInstance().onActivate(1);
        }

        // Code to execute when the application is deactivated (sent to background)
        // This code will not execute when the application is closing
        private void Application_Deactivated(object sender, DeactivatedEventArgs e)
        {
            rhoruntime.CRhoRuntime.getInstance().DestroyUi();
        }

        // Code to execute when the application is closing (eg, user hit Back)
        // This code will not execute when the application is deactivated
        // in this case we have to send app to background, we shouldnt close it
        // confirmed by Evgeniy
        private void Application_Closing(object sender, ClosingEventArgs e)
        {
            rhoruntime.CRhoRuntime.getInstance().DestroyUi();
        }

        // Code to execute if a navigation fails
        private void RootFrame_NavigationFailed(object sender, NavigationFailedEventArgs e)
        {
            if (System.Diagnostics.Debugger.IsAttached)
            {
                // A navigation has failed; break into the debugger
                System.Diagnostics.Debugger.Break();
            }
        }

        // Code to execute on Unhandled Exceptions
        private void Application_UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            // just ignore QuitException
            if (e.ExceptionObject is QuitException)
                return;
            
            if (System.Diagnostics.Debugger.IsAttached)
            {
                // An unhandled exception has occurred; break into the debugger
                System.Diagnostics.Debugger.Break();
            }
        }


        #region Phone application initialization

        // Avoid double-initialization
        private bool phoneApplicationInitialized = false;

        // Do not add any additional code to this method
        private void InitializePhoneApplication()
        {
            if (phoneApplicationInitialized)
                return;

            // Create the frame but don't set it as RootVisual yet; this allows the splash
            // screen to remain active until the application is ready to render.
            RootFrame = new PhoneApplicationFrame();
            RootFrame.Navigated += CompleteInitializePhoneApplication;
			
			RootFrame.UriMapper = new AssociationUriMapper();

            // Handle navigation failures
            RootFrame.NavigationFailed += RootFrame_NavigationFailed;

            // Ensure we don't initialize again
            phoneApplicationInitialized = true;

            Directory.SetCurrentDirectory(Windows.Storage.ApplicationData.Current.LocalFolder.Path);

            CRhoResourceMap.deployContent();
        }

        // Do not add any additional code to this method
        private void CompleteInitializePhoneApplication(object sender, NavigationEventArgs e)
        {
            // Set the root visual to allow the application to render
            if (RootVisual != RootFrame)
                RootVisual = RootFrame;

            // Remove this handler since it is no longer needed
            RootFrame.Navigated -= CompleteInitializePhoneApplication;
        }

        #endregion
    }
}
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
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using rhodes.Resources;
using rhoruntime;

namespace rhodes
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
            try
            {
                var rhoruntime = new CRhoRuntime();
                rhoruntime.SetCallback(new CallbackPointer(RhodesLoaded));
                rhoruntime.DoCallback();
            }
            catch (Exception e)
            {
                RhodesWebBrowser.NavigateToString("<html><head><title>Exception</title></head><body>Exception: "+e.Message+"</body></html>");
            }
        }

        public void RhodesLoaded()
        {
            RhodesWebBrowser.NavigateToString("<html><head><title>Loading...</title></head><body>Loading...</body></html>");
        }

        public void RhodesReloaded() 
        {
            RhodesWebBrowser.NavigateToString("<html><head><title>Reloaded...</title></head><body>Reloaded...</body></html>");
        }
    }
}
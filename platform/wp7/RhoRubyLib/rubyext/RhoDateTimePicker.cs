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

using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using Microsoft.Phone.Tasks;
using rho.common;
using System.Globalization;
using System.Windows;
using Microsoft.Phone.Controls;
using rho.views;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Navigation;
using Microsoft.Phone.Controls.Primitives;


namespace rho.rubyext
{
    [RubyModule("DateTimePicker")]
    public static class RhoDateTimePicker
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoDateTimePicker");
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
        private static PhoneApplicationFrame m_frame = null;
        private static object m_frameContentWhenOpened;
        private static RhoDateTimeDlg m_dateTimePickerPage = null;
        private static DateTime m_dateValue;
        private static DateTime m_timeValue;
        private static String m_callback = "";
        private static String m_title = "";
        private static String m_opaque = null;
        private static int m_fmt = 0;
        

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods


        [RubyMethodAttribute("choose", RubyMethodAttributes.PublicSingleton)]
        public static void choose(RubyModule/*!*/ self, params object[] args)
        {
            try
            {
                if (args != null && args.Length != 4 && args.Length != 5)
                    throw RubyExceptionData.InitializeException(new RuntimeError("wrong number of arguments"), "wrong number of arguments");

                if (args.Length == 5)
                    m_opaque = args[4].ToString();

                m_fmt = int.Parse(args[3].ToString());

                m_callback = args[0].ToString();
                m_title = args[1].ToString();
                //long init = args[2].toRubyTime().getTime();

                RHODESAPP().MainPage.Dispatcher.BeginInvoke(() =>
                {
                    OpenPickerPage();
                });
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("choose", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("choose_with_range", RubyMethodAttributes.PublicSingleton)]
        public static void choose_with_range(RubyModule/*!*/ self, params object[] args)
        {
            try
            {
                if (args != null && args.Length != 4 && args.Length != 5)
                    throw RubyExceptionData.InitializeException(new RuntimeError("wrong number of arguments"), "wrong number of arguments");

                if (args.Length == 5)
                    m_opaque = args[4].ToString();

                m_fmt = int.Parse(args[3].ToString());

                m_callback = args[0].ToString();
                m_title = args[1].ToString();
                //long init = args[2].toRubyTime().getTime();
                //long min_t = args[5].toRubyTime().getTime();
                //long max_t = args[6].toRubyTime().getTime();

                RHODESAPP().MainPage.Dispatcher.BeginInvoke(() =>
                {
                    OpenPickerPage();
                });
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("choose_with_range", ex);
                throw rubyEx;
            }
        }

        #endregion

        #region Helpers

        private static void OpenPickerPage()
        {
            if (null == m_frame)
            {
                m_frame = Application.Current.RootVisual as PhoneApplicationFrame;
                if (null != m_frame)
                {
                    m_frameContentWhenOpened = m_frame.Content;

                    m_frame.Navigated += OnFrameNavigated;
                    m_frame.NavigationStopped += OnFrameNavigationStoppedOrFailed;
                    m_frame.NavigationFailed += OnFrameNavigationStoppedOrFailed;

                    m_frame.Navigate(new Uri("/RhoRubyLib;component/views/RhoDateTimeDlg.xaml", UriKind.Relative));
                    RHODESAPP().m_transition = true;
                }
            }

        }

        private static void ClosePickerPage()
        {
            if (null != m_frame)
            {
                m_frame.Navigated -= OnFrameNavigated;
                m_frame.NavigationStopped -= OnFrameNavigationStoppedOrFailed;
                m_frame.NavigationFailed -= OnFrameNavigationStoppedOrFailed;

                m_frame = null;
                m_frameContentWhenOpened = null;
            }

            if (null != m_dateTimePickerPage)
            {
                m_dateValue = m_dateTimePickerPage.m_dateValue;
                m_timeValue = m_dateTimePickerPage.m_timeValue;
                DateTime result = new DateTime();

                switch(m_fmt)
                {
                    case 0:
                        result = m_dateValue;//TODO// + m_timeValue;
                        break;
                    case 1:
                        result = m_dateValue;
                        break;
                    case 2:
                        result = m_timeValue;
                        break;
                    default:
                        break;
                }

                long sec = (result - new DateTime(1970, 1, 1)).Ticks / 10000000;

                if (m_callback != "")
                {
                    m_callback = RHODESAPP().canonicalizeRhoUrl(m_callback);
                    String body ="";
                    if(m_dateTimePickerPage.m_cancel == false)
                        body += "status=ok&result=" + sec.ToString();
                    else
                        body += "status=cancel";
                    if (m_opaque != null)
                        body += "&opaque=" + m_opaque;
                    body += "&rho_callback=1";
                    RhoClassFactory.createNetRequest().pushData(m_callback, body, null);
                    //m_frame.InvalidateArrange();
                }
                m_dateTimePickerPage = null;
            }
        }

        private static void OnFrameNavigated(object sender, NavigationEventArgs e)
        {
            if (e.Content == m_frameContentWhenOpened)
                ClosePickerPage();
            else if (null == m_dateTimePickerPage)
                m_dateTimePickerPage = e.Content as RhoDateTimeDlg;
        }

        private static void OnFrameNavigationStoppedOrFailed(object sender, EventArgs e)
        {
            ClosePickerPage();
        }

        #endregion

    }
}

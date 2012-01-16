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
using Coding4Fun.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("Alert")]
    public static class RhoAlert
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoAlert");
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
        private static MessagePrompt m_messagePrompt = null;
        private static String m_callback = "";
        private static int m_buttonId = 0;
        private static String m_buttonTitle = "";

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("show_popup", RubyMethodAttributes.PublicSingleton)]
        public static void ShowPopup(RubyModule/*!*/ self, Hash args)
        {
            String message = "";
            String title = "";
            Object[] buttons = null;
            object val = null;

            if (args != null && args.TryGetValue(CRhoRuby.CreateSymbol("title"), out val))
                title = val.ToString();
            if (args != null && args.TryGetValue(CRhoRuby.CreateSymbol("message"), out val))
                message = val.ToString();
            if (args != null && args.TryGetValue(CRhoRuby.CreateSymbol("callback"), out val))
                m_callback = val.ToString();
            if (args != null && args.TryGetValue(CRhoRuby.CreateSymbol("buttons"), out val) && val is RubyArray)
                buttons = ((RubyArray)val).ToArray();

            RHODESAPP().MainPage.Dispatcher.BeginInvoke(() =>
            {
                m_messagePrompt = new MessagePrompt
                {
                    Title = title,
                    Message = message
                };
                m_messagePrompt.Completed += messagePrompt_Completed;
                m_messagePrompt.ActionPopUpButtons.Clear();
                for (int i = 0; buttons != null && i < buttons.Length; i++)
                {
                    if (buttons[i] != null)
                    {
                        Button customButton = new Button() { Content = buttons[i] };
                        customButton.Click += new RoutedEventHandler(customButton_Click);
                        m_messagePrompt.ActionPopUpButtons.Add(customButton);
                    }
                }
                m_messagePrompt.Show();
            });
    
        }

        [RubyMethodAttribute("hide_popup", RubyMethodAttributes.PublicSingleton)]
        public static void HidePopup(RubyModule/*!*/ self)
        {
            RHODESAPP().MainPage.Dispatcher.BeginInvoke(() =>
            {
                if (m_messagePrompt != null)
                    m_messagePrompt.Hide();
            });
        }

        [RubyMethodAttribute("vibrate", RubyMethodAttributes.PublicSingleton)]
        public static void Vibrate(RubyModule/*!*/ self, int duration)
        {
        }

        [RubyMethodAttribute("play_file", RubyMethodAttributes.PublicSingleton)]
        public static void PlayFile(RubyModule/*!*/ self, [NotNull]String/*!*/ fileName, [NotNull]String/*!*/ mediaType)
        {
        }

        [RubyMethodAttribute("show_status", RubyMethodAttributes.PublicSingleton)]
        public static void ShowStatus(RubyModule/*!*/ self, [NotNull]String/*!*/ title, [NotNull]String/*!*/ text, String/*!*/ hideLabel)
        {
        }

        #endregion

        private static void customButton_Click(object sender, RoutedEventArgs e)
        {
            if (m_messagePrompt != null)
            {
                m_buttonTitle = ((Button)sender).Content.ToString();
                m_buttonId = m_messagePrompt.ActionPopUpButtons.IndexOf((Button)sender);
                m_messagePrompt.Hide();
            }
        }

        private static void messagePrompt_Completed(object sender, PopUpEventArgs<string, PopUpResult> e)
        {
            if (m_callback != "")
            {
                m_callback = RHODESAPP().canonicalizeRhoUrl(m_callback);
                RhoClassFactory.createNetRequest().pushData(m_callback, "button_id="+m_buttonId.ToString()+"&button_title="+m_buttonTitle+"&rho_callback=1", null);
            }
        }


    }
}

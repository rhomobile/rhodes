using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("WebView")]
    public static class RhoWebView
    {
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RhodesApp { get { return CRhodesApp.Instance; } }

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static void Navigate(RubyModule/*!*/ self, [NotNull]String/*!*/ url)
        {
            RhoRuby.WebBrowser.Navigate(new Uri(url, UriKind.Relative));
        }

        [RubyMethodAttribute("execute_js", RubyMethodAttributes.PublicSingleton)]
        public static void execute_js(RubyModule/*!*/ self, [NotNull]String/*!*/ strScript)
        {
            RhoRuby.WebBrowser.InvokeScript(strScript);
        }

        [RubyMethodAttribute("current_location", RubyMethodAttributes.PublicSingleton)]
        public static String currentLocation(RubyModule/*!*/ self)
        {
            return RhodesApp.getCurrentUrl(0);   
        }

        #endregion
    }
}

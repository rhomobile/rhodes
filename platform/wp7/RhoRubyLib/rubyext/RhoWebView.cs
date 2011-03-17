using Microsoft.Scripting.Utils;
using IronRuby.Runtime;
using Microsoft.Phone.Controls;
using System;

namespace rho.rubyext
{
    [RubyModule("WebView")]
    public static class RhoWebView
    {
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PrivateInstance)]
        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static bool Navigate(object self, [DefaultProtocol]string url)
        {
            RhoRuby.WebBrowser.Navigate(new Uri(url, UriKind.Relative));
            return true;
        }

        #endregion
    }
}

using Microsoft.Scripting.Utils;
using IronRuby.Runtime;
using Microsoft.Phone.Controls;
using System;

namespace RhoRuby
{
    [RubyModule("WebView")]
    public static class RhoWebView
    {

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PrivateInstance)]
        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static bool Navigate(object self, [DefaultProtocol]string url)
        {
            RhoRubyFramework.WebBrowser.Navigate(new Uri(url, UriKind.Relative));
            return true;
        }

        #endregion
    }
}

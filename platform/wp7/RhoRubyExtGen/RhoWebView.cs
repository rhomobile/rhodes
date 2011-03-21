using Microsoft.Scripting.Utils;
using IronRuby.Runtime;
using System;

namespace rho.rubyext
{
    [RubyModule("WebView")]
    public static class RhoWebView
    {
        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PrivateInstance)]
        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static void Navigate(object self, [DefaultProtocol]string url)
        {
        }

        #endregion
    }
}

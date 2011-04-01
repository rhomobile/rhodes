using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;

namespace rho.rubyext
{
    [RubyModule("WebView")]
    public static class RhoWebView
    {
        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static void Navigate(RubyModule/*!*/ self, [NotNull]String/*!*/ url)
        {
        }

        [RubyMethodAttribute("execute_js", RubyMethodAttributes.PublicSingleton)]
        public static void execute_js(RubyModule/*!*/ self, [NotNull]String/*!*/ strScript)
        {
        }

        #endregion
    }
}

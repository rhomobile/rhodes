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
        public static void Navigate(RubyModule/*!*/ self, [NotNull]String/*!*/ url, int index = 0)
        {
        }

        [RubyMethodAttribute("refresh", RubyMethodAttributes.PublicSingleton)]
        public static void Refresh(RubyModule/*!*/ self, int index = 0)
        {
        }

        [RubyMethodAttribute("execute_js", RubyMethodAttributes.PublicSingleton)]
        public static void execute_js(RubyModule/*!*/ self, [NotNull]String/*!*/ strScript, int index = 0)
        {
        }

        [RubyMethodAttribute("current_location", RubyMethodAttributes.PublicSingleton)]
        public static String currentLocation(RubyModule/*!*/ self)
        {
            return null;
        }

        [RubyMethodAttribute("set_menu_items", RubyMethodAttributes.PublicSingleton)]
        public static void setMenuItems(RubyModule/*!*/ self, Hash valMenu)
        {
        }

        #endregion
    }
}

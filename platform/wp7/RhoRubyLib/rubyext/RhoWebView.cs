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
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoWebView");
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("navigate", RubyMethodAttributes.PublicSingleton)]
        public static void Navigate(RubyModule/*!*/ self, [NotNull]String/*!*/ url)
        {
            try
            {
                RHODESAPP().processWebNavigate(url);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("navigate", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("execute_js", RubyMethodAttributes.PublicSingleton)]
        public static void execute_js(RubyModule/*!*/ self, [NotNull]String/*!*/ strScript)
        {
            try
            {
                RHODESAPP().processInvokeScript(strScript);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("execute_js", ex);
                throw rubyEx;
            }
        }

        [RubyMethodAttribute("current_location", RubyMethodAttributes.PublicSingleton)]
        public static String currentLocation(RubyModule/*!*/ self)
        {
            String res = "";
            try
            {
                res = RHODESAPP().getCurrentUrl(0);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("current_location", ex);
                throw rubyEx;
            }

            return res;   
        }

        [RubyMethodAttribute("set_menu_items", RubyMethodAttributes.PublicSingleton)]
        public static void setMenuItems(RubyModule/*!*/ self, Hash valMenu)
        {
            try
            {
                RHODESAPP().setMenuItems(valMenu);
            }
            catch (Exception ex)
            {
                Exception rubyEx = self.Context.CurrentException;
                if (rubyEx == null)
                {
                    rubyEx = RubyExceptionData.InitializeException(new RuntimeError(ex.Message.ToString()), ex.Message);
                }
                LOG.ERROR("set_menu_items", ex);
                throw rubyEx;
            }
        }

        #endregion
    }
}

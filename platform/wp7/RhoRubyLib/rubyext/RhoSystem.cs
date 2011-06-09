using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("System")]
    public static class RhoSystem
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("RhoSystem");
        private static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }

        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("get_property", RubyMethodAttributes.PublicSingleton)]
        public static Object get_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            if(name == "platform") return "WP7";
            else if(name == "os_version") return "7.0";
            else if(name == "country") return "us";
            else return "";
        }

        [RubyMethodAttribute("has_network", RubyMethodAttributes.PublicSingleton)]
        public static Boolean has_network(RubyModule/*!*/ self)
        {
            return true;
        }

        [RubyMethodAttribute("get_locale", RubyMethodAttributes.PublicSingleton)]
        public static String get_locale(RubyModule/*!*/ self)
        {
            return "en";
        }

        [RubyMethodAttribute("get_screen_width", RubyMethodAttributes.PublicSingleton)]
        public static int get_screen_width(RubyModule/*!*/ self)
        {
            return 0;
        }

        [RubyMethodAttribute("get_screen_height", RubyMethodAttributes.PublicSingleton)]
        public static int get_screen_height(RubyModule/*!*/ self)
        {
            return 0;
        }

        [RubyMethodAttribute("set_push_notification", RubyMethodAttributes.PublicSingleton)]
        public static void set_push_notification(RubyModule/*!*/ self, [NotNull]String/*!*/ url, [NotNull]String/*!*/ _params)
        {
        }

        [RubyMethodAttribute("set_screen_rotation_notification", RubyMethodAttributes.PublicSingleton)]
        public static void set_screen_rotation_notification(RubyModule/*!*/ self, [NotNull]String/*!*/ url, [NotNull]String/*!*/ _params)
        {
        }

        [RubyMethodAttribute("exit", RubyMethodAttributes.PublicSingleton)]
        public static void exit(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("set_sleeping", RubyMethodAttributes.PublicSingleton)]
        public static void set_sleeping(RubyModule/*!*/ self, bool value)
        {
        }

        [RubyMethodAttribute("app_installed?", RubyMethodAttributes.PublicSingleton)]
        public static bool is_app_installed(RubyModule/*!*/ self, [NotNull]String/*!*/ app_name)
        {
            return false;
        }

        [RubyMethodAttribute("app_install", RubyMethodAttributes.PublicSingleton)]
        public static void app_install(RubyModule/*!*/ self, [NotNull]String/*!*/ url)
        {
        }

        [RubyMethodAttribute("app_uninstall", RubyMethodAttributes.PublicSingleton)]
        public static void app_uninstall(RubyModule/*!*/ self, [NotNull]String/*!*/ app_name)
        {
        }

        [RubyMethodAttribute("run_app", RubyMethodAttributes.PublicSingleton)]
        public static void run_app(RubyModule/*!*/ self, [NotNull]String/*!*/ app_name, String _params)
        {
        }

        [RubyMethodAttribute("open_url", RubyMethodAttributes.PublicSingleton)]
        public static void open_url(RubyModule/*!*/ self, [NotNull]String/*!*/ url)
        {
        }

        [RubyMethodAttribute("unzip_file", RubyMethodAttributes.PublicSingleton)]
        public static void unzip_file(RubyModule/*!*/ self, [NotNull]String/*!*/ path)
        {
        }

        [RubyMethodAttribute("get_start_params", RubyMethodAttributes.PublicSingleton)]
        public static String get_start_params(RubyModule/*!*/ self, [NotNull]String/*!*/ path)
        {
            return null;
        }

        [RubyMethodAttribute("start_timer", RubyMethodAttributes.PublicSingleton)]
        public static void start_timer(RubyModule/*!*/ self, int interval, [NotNull]String/*!*/ url, String _params)
        {
        }

        [RubyMethodAttribute("stop_timer", RubyMethodAttributes.PublicSingleton)]
        public static void stop_timer(RubyModule/*!*/ self, [NotNull]String/*!*/ url)
        {
        }

        #endregion
    }
}

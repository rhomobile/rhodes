using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;

namespace rho.rubyext
{
    [RubyModule("RhoConf")]
    public static class RhoConfig
    {
        #region Private Implementation Details

        #endregion

        #region Private Instance & Singleton Methods

        [RubyMethodAttribute("set_property_by_name", RubyMethodAttributes.PublicSingleton)]
        public static void set_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name, Object value)
        {
        }

        [RubyMethodAttribute("get_property_by_name", RubyMethodAttributes.PublicSingleton)]
        public static Object get_property_by_name(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            return null;
        }

        [RubyMethodAttribute("is_property_exists", RubyMethodAttributes.PublicSingleton)]
        public static Boolean is_property_exists(RubyModule/*!*/ self, [NotNull]String/*!*/ name)
        {
            return true;
        }

        [RubyMethodAttribute("show_log", RubyMethodAttributes.PublicSingleton)]
        public static void show_log(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("send_log", RubyMethodAttributes.PublicSingleton)]
        public static void send_log(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("clean_log", RubyMethodAttributes.PublicSingleton)]
        public static void clean_log(RubyModule/*!*/ self)
        {
        }

        [RubyMethodAttribute("read_log", RubyMethodAttributes.PublicSingleton)]
        public static void read_log(RubyModule/*!*/ self, int limit = 0)
        {
        }

        #endregion
    }
}

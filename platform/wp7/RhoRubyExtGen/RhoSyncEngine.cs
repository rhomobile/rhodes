using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Runtime.InteropServices;

namespace rho.rubyext
{
    [RubyModule("SyncEngine")]
    public static class RhoSyncEngine
    {
        [RubyMethod("dosync", RubyMethodAttributes.PublicSingleton)]
        public static object dosync(RubyModule/*!*/ self)
        {
            return null;
        }

        [RubyMethod("dosync", RubyMethodAttributes.PublicSingleton)]
        public static object dosync(RubyModule/*!*/ self, bool/*!*/ show_status_popup)
        {
            return null;
        }

        [RubyMethod("dosync_source", RubyMethodAttributes.PublicSingleton)]
        public static object dosync_source(RubyModule/*!*/ self, [NotNull]object/*!*/ srcID)
        {
            return null;
        }

        [RubyMethod("dosync_source", RubyMethodAttributes.PublicSingleton)]
        public static object dosync_source(RubyModule/*!*/ self, [NotNull]object/*!*/ srcID, bool/*!*/ show_status_popup)
        {
            return null;
        }

        [RubyMethod("logged_in", RubyMethodAttributes.PublicSingleton)]
        public static int logged_in(RubyModule/*!*/ self)
        {
            return 0;
        }

        [RubyMethod("login", RubyMethodAttributes.PublicSingleton)]
        public static object login(RubyModule/*!*/ self, [NotNull]String/*!*/ login, [NotNull]String/*!*/ pwd, [NotNull]String/*!*/ callback)
        {
            return null;
        }

        [RubyMethod("set_notification", RubyMethodAttributes.PublicSingleton)]
        public static void set_notification(RubyModule/*!*/ self, [NotNull]int/*!*/ srcID, [NotNull]String/*!*/ url, string/*!*/ callback_param)
        {
        }

        [RubyMethod("enable_status_popup", RubyMethodAttributes.PublicSingleton)]
        public static void enable_status_popup(RubyModule/*!*/ self, bool/*!*/ enable)
        {
        }

        [RubyMethod("is_blob_attr", RubyMethodAttributes.PublicSingleton)]
        public static bool is_blob_attr(RubyModule/*!*/ self, [NotNull]String/*!*/ szPartition, [NotNull]int/*!*/ srcID, [NotNull]String/*!*/ szAttrName)
        {
            return false;
        }

        [RubyMethod("update_blob_attribs", RubyMethodAttributes.PublicSingleton)]
        public static void update_blob_attribs(RubyModule/*!*/ self, [NotNull]String/*!*/ szPartition, [NotNull]int/*!*/ srcID)
        {
        }

        [RubyMethod("set_threaded_mode", RubyMethodAttributes.PublicSingleton)]
        public static void set_threaded_mode(RubyModule/*!*/ self, [NotNull]bool/*!*/ bThreadMode)
        {
        }

        [RubyMethod("logout", RubyMethodAttributes.PublicSingleton)]
        public static void logout(RubyModule/*!*/ self)
        {
        }

        [RubyMethod("set_syncserver", RubyMethodAttributes.PublicSingleton)]
        public static void set_syncserver(RubyModule/*!*/ self, [NotNull]String/*!*/ syncserver)
        {
        }

        [RubyMethod("set_pollinterval", RubyMethodAttributes.PublicSingleton)]
        public static int set_pollinterval(RubyModule/*!*/ self, [NotNull]int/*!*/ nInterval)
        {
            return 0;
        }

        [RubyMethod("stop_sync", RubyMethodAttributes.PublicSingleton)]
        public static void set_pollinterval(RubyModule/*!*/ self)
        {
        }

        [RubyMethod("set_source_property", RubyMethodAttributes.PublicSingleton)]
        public static void set_source_property(RubyModule/*!*/ self, [NotNull]int/*!*/ srcID, [NotNull]String/*!*/ szPropName, [NotNull]String/*!*/ szPropValue )
        {
        }        
        
        [RubyMethod("set_objectnotify_url", RubyMethodAttributes.PublicSingleton)]
        public static void set_objectnotify_url(RubyModule/*!*/ self, [NotNull]string/*!*/ url)
        {
        }
    }
}
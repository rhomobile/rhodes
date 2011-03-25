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

        [RubyMethod("is_blob_attr", RubyMethodAttributes.PublicSingleton)]
        public static long is_blob_attr(RubyModule/*!*/ self, MutableString szPartition, int srcID, MutableString szAttrName)
        {
            return 0;
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

        [RubyMethod("update_blob_attribs", RubyMethodAttributes.PublicSingleton)]
        public static void update_blob_attribs(RubyModule/*!*/ self, [NotNull]String/*!*/ szPartition, [NotNull]int/*!*/ srcID)
        {
        }
        

        [RubyMethod("set_objectnotify_url", RubyMethodAttributes.PublicSingleton)]
        public static void set_objectnotify_url(RubyModule/*!*/ self, [NotNull]string/*!*/ url)
        {
        }
    }
}
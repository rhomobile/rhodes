using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using rho.sync;
using rho.common;
using rho.db;

namespace rho.rubyext
{
    [RubyModule("SyncEngine")]
    public static class RhoSyncEngine
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
                    new RhoLogger("Sync");
        static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

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
        public static object login(RubyModule/*!*/ self, [NotNull]String/*!*/ name, [NotNull]String/*!*/ pwd, [NotNull]String/*!*/ callback)
        {
            try{
                SyncThread.stopSync();

                SyncThread.getInstance().addQueueCommand(new SyncThread.SyncLoginCommand(name, pwd, callback,
                        new SyncNotify.SyncNotification(callback, "", false)));
                return SyncThread.getInstance().getRetValue();
            }
            catch (Exception e)
            {
                LOG.ERROR("SyncEngine.login", e);
                RhoRuby.raise_RhoError(RhoAppAdapter.ERR_RUNTIME);
            }

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
        public static void update_blob_attribs(RubyModule/*!*/ self, [NotNull]String/*!*/ strPartition, [NotNull]int/*!*/ srcID)
        {
            DBAdapter db = DBAdapter.getDB(strPartition);
            db.getAttrMgr().loadBlobAttrs(db);
        }

        [RubyMethod("set_objectnotify_url", RubyMethodAttributes.PublicSingleton)]
        public static void set_objectnotify_url(RubyModule/*!*/ self, [DefaultProtocol, NotNull]string/*!*/ url)
        {
        }
    }
}
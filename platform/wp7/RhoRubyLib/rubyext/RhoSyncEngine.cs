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
            SyncThread.getInstance().addQueueCommand(new SyncThread.SyncCommand(SyncThread.scSyncAll, false));
            return SyncThread.getInstance().getRetValue();
        }

        [RubyMethod("dosync", RubyMethodAttributes.PublicSingleton)]
        public static object dosync(RubyModule/*!*/ self, bool/*!*/ show_status_popup)
        {
            SyncThread.getInstance().addQueueCommand(new SyncThread.SyncCommand(SyncThread.scSyncAll, show_status_popup));
            return SyncThread.getInstance().getRetValue();
        }

        [RubyMethod("dosync_source", RubyMethodAttributes.PublicSingleton)]
        public static object dosync_source(RubyModule/*!*/ self, [NotNull]object/*!*/ srcID)
        {
            //TODO: dosync_source
            return null;
        }

        [RubyMethod("dosync_source", RubyMethodAttributes.PublicSingleton)]
        public static object dosync_source(RubyModule/*!*/ self, [NotNull]object/*!*/ srcID, bool/*!*/ show_status_popup)
        {
            //TODO: dosync_source
            return null;
        }

        [RubyMethod("logged_in", RubyMethodAttributes.PublicSingleton)]
        public static int logged_in(RubyModule/*!*/ self)
        {
            return SyncThread.getSyncEngine().isLoggedIn() ? 1 : 0;
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
        public static void set_notification(RubyModule/*!*/ self, [NotNull]int/*!*/ nSrcID, [NotNull]String/*!*/ url, string/*!*/ callback_param)
        {
            SyncThread.getSyncEngine().getNotify().setSyncNotification(nSrcID,
                new SyncNotify.SyncNotification(url, callback_param != null ? callback_param : "", nSrcID != -1));
        }

        [RubyMethod("enable_status_popup", RubyMethodAttributes.PublicSingleton)]
        public static void enable_status_popup(RubyModule/*!*/ self, bool/*!*/ bEnable)
        {
            SyncThread.getSyncEngine().getNotify().enableStatusPopup(bEnable);
        }

        [RubyMethod("is_blob_attr", RubyMethodAttributes.PublicSingleton)]
        public static bool is_blob_attr(RubyModule/*!*/ self, [NotNull]String/*!*/ strPartition, [NotNull]int/*!*/ nSrcID, [NotNull]String/*!*/ strAttrName)
        {
            return DBAdapter.getDB(strPartition).getAttrMgr().isBlobAttr(nSrcID, strAttrName);
        }

        [RubyMethod("update_blob_attribs", RubyMethodAttributes.PublicSingleton)]
        public static void update_blob_attribs(RubyModule/*!*/ self, [NotNull]String/*!*/ strPartition, [NotNull]int/*!*/ srcID)
        {
            DBAdapter db = DBAdapter.getDB(strPartition);
            db.getAttrMgr().loadBlobAttrs(db);
        }

        [RubyMethod("set_pollinterval", RubyMethodAttributes.PublicSingleton)]
        public static int set_pollinterval(RubyModule/*!*/ self, [NotNull]int/*!*/ nInterval)
        {
            int nOldInterval = SyncThread.getInstance().getPollInterval();
            SyncThread.getInstance().setPollInterval(nInterval);
            return nOldInterval;
        }

        [RubyMethod("stop_sync", RubyMethodAttributes.PublicSingleton)]
        public static void set_pollinterval(RubyModule/*!*/ self)
        {
            SyncThread.stopSync();
        }

        [RubyMethod("set_source_property", RubyMethodAttributes.PublicSingleton)]
        public static void set_source_property(RubyModule/*!*/ self, [NotNull]int/*!*/ nSrcID, [NotNull]String/*!*/ strPropName, [NotNull]String/*!*/ strPropValue)
        {
            SyncEngine.getSourceOptions().setProperty(nSrcID, strPropName, strPropValue);
        }

        [RubyMethod("set_objectnotify_url", RubyMethodAttributes.PublicSingleton)]
        public static void set_objectnotify_url(RubyModule/*!*/ self, [DefaultProtocol, NotNull]string/*!*/ url)
        {
            SyncNotify.setObjectNotifyUrl(url);
        }
    }
}
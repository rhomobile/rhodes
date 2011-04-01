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
            int nSrcID = 0;
			String strName = "";
            if (srcID is long)
                nSrcID = (int)(long)srcID;
            else if (srcID is int)
                nSrcID = (int)srcID;
            else
                strName = srcID.ToString();

            SyncThread.getInstance().addQueueCommand(new SyncThread.SyncCommand(SyncThread.scSyncOne, strName, nSrcID, true));
            return SyncThread.getInstance().getRetValue();
        }
        
        [RubyMethod("dosync_source", RubyMethodAttributes.PublicSingleton)]
        public static object dosync_source(RubyModule/*!*/ self, [NotNull]object/*!*/ srcID, bool/*!*/ show_status_popup)
        {
            int nSrcID = 0;
            String strName = "";
            if (srcID is long)
                nSrcID = (int)(long)srcID;
            else if (srcID is int)
                nSrcID = (int)srcID;
            else
                strName = srcID.ToString();

            SyncThread.getInstance().addQueueCommand(new SyncThread.SyncCommand(SyncThread.scSyncOne, strName, nSrcID, show_status_popup));
            return SyncThread.getInstance().getRetValue();
        }

        [RubyMethod("dosearch", RubyMethodAttributes.PublicSingleton)]
        public static object dosearch(RubyModule/*!*/ self, [NotNull]RubyArray/*!*/ arSourcesR, [NotNull]String/*!*/ from, [NotNull]String/*!*/ strParams,
            [NotNull]bool/*!*/ bSearchSyncChanges, [NotNull]int/*!*/ nProgressStep, String/*!*/ strCallback, String/*!*/ strCallbackParams)
        {
            SyncThread.stopSync();

			if ( strCallback != null && strCallback.Length > 0 )
				SyncThread.getSyncEngine().getNotify().setSearchNotification(strCallback, strCallbackParams);

            Vector<String> arSources = RhoRuby.makeVectorStringFromArray(arSourcesR);

            SyncThread.getInstance().addQueueCommand(new SyncThread.SyncSearchCommand(from, strParams, arSources, bSearchSyncChanges, nProgressStep));        

            return SyncThread.getInstance().getRetValue();
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

        [RubyMethod("set_threaded_mode", RubyMethodAttributes.PublicSingleton)]
        public static void set_threaded_mode(RubyModule/*!*/ self, [NotNull]bool/*!*/ bThreadMode)
        {
            SyncThread.getInstance().setNonThreadedMode(!bThreadMode);
            SyncThread.getSyncEngine().setNonThreadedMode(!bThreadMode);
        }

        [RubyMethod("logout", RubyMethodAttributes.PublicSingleton)]
        public static void logout(RubyModule/*!*/ self)
        {
            SyncThread.stopSync();
            SyncThread.getSyncEngine().stopSyncByUser();
            SyncThread.getSyncEngine().logout();
        }

        [RubyMethod("set_syncserver", RubyMethodAttributes.PublicSingleton)]
        public static void set_syncserver(RubyModule/*!*/ self, [NotNull]String/*!*/ syncserver)
        {
            SyncThread.stopSync();
            SyncThread.getSyncEngine().setSyncServer(syncserver);

            if (syncserver != null && syncserver.length() > 0)
            {
                SyncThread.getInstance().start(SyncThread.epLow);
                if (ClientRegister.getInstance() != null)
                    ClientRegister.getInstance().startUp();
            }
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

        [RubyMethod("add_objectnotify", RubyMethodAttributes.PublicSingleton)]
        public static void add_objectnotify(RubyModule/*!*/ self, [NotNull]int/*!*/ nSrcID, [NotNull]string/*!*/ strObject)
        {
            SyncThread.getSyncEngine().getNotify().addObjectNotify(nSrcID, strObject);
        }

        [RubyMethod("clean_objectnotify", RubyMethodAttributes.PublicSingleton)]
        public static void clean_objectnotify(RubyModule/*!*/ self)
        {
            SyncThread.getSyncEngine().getNotify().cleanObjectNotifications();
        }
    }
}
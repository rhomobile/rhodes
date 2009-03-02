package com.rho.sync;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyTwoArgMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

/**
 * The Class SyncEngine.
 */
public class SyncEngine extends RubyBasic {

	/** The s thread. */
	private static SyncThread sThread = null;
	private static SyncNotifications sNotifications;

	// @RubyAllocMethod
	/**
	 * Alloc.
	 * 
	 * @param receiver the receiver
	 * 
	 * @return the sync engine
	 */
	public static SyncEngine alloc(RubyValue receiver) {
		return new SyncEngine(RubyRuntime.SyncEngineClass);
	}

	public static void setNotificationImpl(SyncNotifications pNotify ){
		sNotifications = pNotify;
	}
	public static SyncNotifications getNotificationImpl(){
		return sNotifications;
	}

	// @RubyLevelMethod(name="dosync")
	/**
	 * Dosync.
	 * 
	 * @param receiver the receiver
	 * 
	 * @return the ruby value
	 */
	public static RubyValue dosync(RubyValue receiver) {
		wakeUp();
		return RubyConstant.QTRUE;
	}

	public static RubyValue login(RubyValue arg1, RubyValue arg2) {
		boolean bRes = SyncUtil.fetch_client_login(arg1.toString(), arg2.toString());
		return bRes ? SyncUtil.createInteger(1L) : SyncUtil.createInteger(0L); 
	}
	
	public static RubyValue logged_in() {
		boolean bRes = SyncUtil.logged_in();
		return bRes ? SyncUtil.createInteger(1L) : SyncUtil.createInteger(0L); 
	}
	
	public static RubyValue logout() {
		SyncUtil.logout();
		return RubyConstant.QNIL; 
	}
	
	public static RubyValue reset_sync_db() {
		SyncThread.setDbResetDelay(true);
		wakeUp();
		return RubyConstant.QNIL; 
	}

	public static RubyValue set_notification(RubyValue arg1, RubyValue arg2) {
		int nSourceID = arg1.toInt();
		String url = arg2.toStr();
		
		sNotifications.setNotification(nSourceID, url);
		return RubyConstant.QNIL;
	}

	public static RubyValue clear_notification(RubyValue arg1) {
		int nSourceID = arg1.toInt();
		
		sNotifications.clearNotification(nSourceID);
		return RubyConstant.QNIL;
	}

	
	/**
	 * Inits the methods.
	 * 
	 * @param klass the klass
	 */
	public static void initMethods(RubyClass klass) {
		klass.defineMethod("initialize", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return ((SyncEngine) receiver).initialize();
			}
		});
		klass.defineAllocMethod(new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return SyncEngine.alloc(receiver);
			}
		});
		klass.getSingletonClass().defineMethod("start", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return SyncEngine.start(receiver);
			}
		});
		klass.getSingletonClass().defineMethod("stop", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return SyncEngine.stop(receiver);
			}
		});
		klass.getSingletonClass().defineMethod("dosync", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return SyncEngine.dosync(receiver);
			}
		});
		klass.getSingletonClass().defineMethod("lock_sync_mutex",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						return SyncEngine.lock_sync_mutex(receiver);
					}
				});
		klass.getSingletonClass().defineMethod("unlock_sync_mutex",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						return SyncEngine.unlock_sync_mutex(receiver);
					}
				});
		klass.getSingletonClass().defineMethod("login",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
						return SyncEngine.login(arg1,arg2);
					}
				});
		
		klass.getSingletonClass().defineMethod("logged_in",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						return SyncEngine.logged_in();
					}
				});
		
		klass.getSingletonClass().defineMethod("logout",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						return SyncEngine.logout();
					}
				});
		
		klass.getSingletonClass().defineMethod("trigger_sync_db_reset",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						return SyncEngine.reset_sync_db();
					}
				});

		klass.getSingletonClass().defineMethod("set_notification",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
						return SyncEngine.set_notification(arg1,arg2);
					}
				});
		klass.getSingletonClass().defineMethod("clear_notification",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
						return SyncEngine.clear_notification(arg1);
					}
				});

	}

	// @RubyLevelMethod(name="lock_sync_mutex")
	/**
	 * Lock_sync_mutex.
	 * 
	 * @param receiver the receiver
	 * 
	 * @return the ruby value
	 */
	public static RubyValue lock_sync_mutex(RubyValue receiver) {
		return RubyConstant.QFALSE;
	}

	// @RubyLevelMethod(name="start")
	/**
	 * Start.
	 * 
	 * @param receiver the receiver
	 * 
	 * @return the ruby value
	 */
	public static RubyValue start(RubyValue receiver) {
		// Initialize only one thread
		if (sThread == null) {
			sThread = new SyncThread();
		}
		return RubyConstant.QTRUE;
	}

	public static void wakeUp() {
		// Initialize only one thread
		if (sThread != null) {
			sThread.wakeUpSyncEngine();
		}
	}
	
	// @RubyLevelMethod(name="stop")
	/**
	 * Stop.
	 * 
	 * @param receiver the receiver
	 * 
	 * @return the ruby value
	 */
	public static RubyValue stop(RubyValue receiver) {
		if ( sThread != null ){
			sThread.quit();
			sThread = null;
		}
		
		return RubyConstant.QTRUE;
	}

	// @RubyLevelMethod(name="unlock_sync_mutex")
	/**
	 * Unlock_sync_mutex.
	 * 
	 * @param receiver the receiver
	 * 
	 * @return the ruby value
	 */
	public static RubyValue unlock_sync_mutex(RubyValue receiver) {
		return RubyConstant.QFALSE;
	}

	/**
	 * Instantiates a new sync engine.
	 * 
	 * @param c the c
	 */
	SyncEngine(RubyClass c) {
		super(c);
	}

	// @RubyLevelMethod(name="initialize")
	/**
	 * Initialize.
	 * 
	 * @return the sync engine
	 */
	public SyncEngine initialize() {
		return this;
	}
}


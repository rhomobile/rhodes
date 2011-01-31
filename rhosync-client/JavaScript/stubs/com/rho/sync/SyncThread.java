package com.rho.sync;

import com.google.gwt.core.client.EntryPoint;
import com.rho.RhoClassFactory;
import com.rho.RhoConf;

public class SyncThread implements EntryPoint {

	private static SyncThread m_syncThreadInstance;
	private SyncEngine m_syncEngine;

	static {
		m_syncThreadInstance = new SyncThread();
	}
	
	protected SyncThread() {
		RhoConf.InitRhoConf();
		RhoConf.getInstance().loadConf();
		m_syncEngine = new SyncEngine();
		try {
			m_syncEngine.setFactory(new RhoClassFactory());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			//e.printStackTrace();
		}
	}
	
	public static SyncThread getInstance() {
		return m_syncThreadInstance;
	}

	public void login(String login, String password, SyncNotify.SyncNotification oNotify) {
		m_syncEngine.login(login, password, oNotify);
	}

	public void logout() {
		try {
			m_syncEngine.logout();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void start() {
		
	}
	
	public void stop() {
		
	}
	
	public void sleep(int i) {
		// TODO Auto-generated method stub
		
	}

	public void wait(int i) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onModuleLoad() {
		exportApiMethods();
	};

	// Export methods to raw javascript API
	public native void exportApiMethods() /*-{
		var me = this;
		var klass = {};
		
		klass.login = function(login, password, notify) {
			me.@com.rho.sync.SyncThread::login(Ljava/lang/String;Ljava/lang/String;Lcom/rho/sync/SyncNotify$SyncNotification;)(login, password, notify);
		};

		klass.logout = function() {
			me.@com.rho.sync.SyncThread::logout();
		};

		klass.start = function() {
			me.@com.rho.sync.SyncThread::start();
		};

		klass.stop = function() {
			me.@com.rho.sync.SyncThread::start();
		};

		$wnd.Rhomobile.sync.Thread = klass;
	}-*/;
}

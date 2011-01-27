package com.rho.sync;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;

public class SyncThread {

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

}

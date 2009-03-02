package com.rho.sync;

import java.util.Hashtable;

public class SyncNotifications {
	private Hashtable m_idToUrlmap = new Hashtable();
	
	void setNotification(int nSourceID, String url){
		m_idToUrlmap.put(new Integer(nSourceID), url);
	}

	void clearNotification(int nSourceID){
		m_idToUrlmap.remove(new Integer(nSourceID));
	}
	
	void fireNotification(int nSourceID){
		String url = (String)m_idToUrlmap.get(new Integer(nSourceID));
		refreshIfCurrent(url);
	}
	
	public void refreshIfCurrent(String url){
		
	}
}

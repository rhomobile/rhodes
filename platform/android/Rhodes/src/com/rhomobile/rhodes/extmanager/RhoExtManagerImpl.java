package com.rhomobile.rhodes.extmanager;

import java.util.Enumeration;
import java.util.Hashtable;

class RhoExtManagerImpl extends Object implements IRhoExtManager {

	private Hashtable<String, IRhoExtension> mExtensions;
	
	public IRhoExtension getExtByName(String strName) {
		return mExtensions.get(strName);
	}

	public void registerExtension(String strName, IRhoExtension ext) {
		if (mExtensions.containsKey(strName)) {
			mExtensions.remove(strName);
		}
		mExtensions.put(strName, ext);
	}
	
	public RhoExtManagerImpl() {
		mExtensions = new Hashtable<String, IRhoExtension>();
	}
	
	public void onBeforeNavigate(int tab_index) {
		Enumeration<String> keys = mExtensions.keys();
		while (keys.hasMoreElements()) {
			String key = keys.nextElement();
			IRhoExtension ext = mExtensions.get(key);
			ext.onBeforeNavigate(tab_index);
		}
	}
	
}
package com.rhomobile.rhodes.extmanager;

public class RhoExtManagerPrivate extends Object {
	
	public static void onBeforeNavigate(int tab_index) {
		RhoExtManagerImpl mngr = (RhoExtManagerImpl)RhoExtManagerSingletone.getRhoExtManagerInstance();
		mngr.onBeforeNavigate(tab_index);
	}
	
	public static void onSetProperty(String extName, String name, String value, int tab_index) {
		RhoExtManagerImpl mngr = (RhoExtManagerImpl)RhoExtManagerSingletone.getRhoExtManagerInstance();
		mngr.onSetProperty(extName, name, value, tab_index);
	}
}

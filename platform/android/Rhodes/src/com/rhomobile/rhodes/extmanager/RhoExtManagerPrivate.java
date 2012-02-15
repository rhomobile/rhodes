package com.rhomobile.rhodes.extmanager;

public class RhoExtManagerPrivate extends Object {
	
	public static void onBeforeNavigate(int tab_index) {
		RhoExtManagerImpl mngr = (RhoExtManagerImpl)RhoExtManagerSingletone.getRhoExtManagerInstance();
		mngr.onBeforeNavigate(tab_index);
	}
	
}

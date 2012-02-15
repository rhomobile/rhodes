package com.rhomobile.rhodes.extmanager;

public class RhoExtManagerSingletone extends Object {
	
	private static RhoExtManagerImpl ourRhoExtManager = null;
	
	
	public static IRhoExtManager getRhoExtManagerInstance() {
		if (ourRhoExtManager == null) {
			ourRhoExtManager = new RhoExtManagerImpl();
		}
		return ourRhoExtManager;
	}
	
}
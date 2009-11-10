package com.rhomobile.rhodes;

public class RhodesInstance {
	
	private static Rhodes instance;

	public static Rhodes getInstance() {
		return instance;
	}

	public static void setInstance(Rhodes instance) {
		if ( RhodesInstance.instance == null )
			RhodesInstance.instance = instance;
	}
	
	public static RhoLogConf getLogConf() {
		return getInstance().getLogConf();
	}
	
}

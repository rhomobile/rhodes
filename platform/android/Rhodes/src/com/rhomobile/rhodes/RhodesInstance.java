package com.rhomobile.rhodes;

public class RhodesInstance {
	
	private static Platform instance;

	public static Platform getInstance() {
		return instance;
	}

	public static void setInstance(Platform instance) {
		if ( RhodesInstance.instance == null )
			RhodesInstance.instance = instance;
	}
	
}

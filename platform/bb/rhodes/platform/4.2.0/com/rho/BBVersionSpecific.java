package com.rho;

public class BBVersionSpecific {
	public static boolean isWifiActive() {
		// BB 4.2.0 does not support detecting WiFi state
		return false;
	}
}

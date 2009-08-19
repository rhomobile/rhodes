package com.rho;

import net.rim.device.api.system.RadioInfo;

public class BBVersionSpecific {
	public static boolean isWifiActive() {
		// BB 4.2.2: detecting WiFi: use getActiveWAFs
		int wafs = RadioInfo.getActiveWAFs();
		return (wafs & RadioInfo.WAF_WLAN) != 0;
	}
}

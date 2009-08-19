package com.rho;

import net.rim.device.api.system.WLANInfo;

public class BBVersionSpecific {
	public static boolean isWifiActive() {
		// BB 4.3+: detecting WiFi: use getWLANState
		int state = WLANInfo.getWLANState();
		return state == WLANInfo.WLAN_STATE_CONNECTED;
	}
}

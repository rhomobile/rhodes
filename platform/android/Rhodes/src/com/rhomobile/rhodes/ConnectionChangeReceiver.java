package com.rhomobile.rhodes;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;

public class ConnectionChangeReceiver extends BroadcastReceiver {
	private static final String TAG = ConnectionChangeReceiver.class.getSimpleName();
	
	private static final int NETWORK_CONNECTED = 1;
	private static final int NETWORK_DISCONNECTED = 2;

	@Override
	public void onReceive(Context context, Intent intent) {

		ConnectivityManager connectivityManager = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo activeNetInfo = connectivityManager.getActiveNetworkInfo();
		
		boolean connected = (activeNetInfo != null) && (activeNetInfo.isConnected());
		
		Logger.I(TAG, "LOGGER: Network status change, connected = " + String.valueOf(connected) );
		RhodesService.getInstance().notifyNetworkStatusChanged(connected?NETWORK_CONNECTED:NETWORK_DISCONNECTED);
	}
}
package com.rhomobile.rhodes;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

public class PushService {

	private static final String TAG = "PushService";
	
	public static final String C2DM_INTENT_PREFIX = "com.google.android.c2dm.intent.";
	
	private static void checkCapabilities() throws IllegalAccessException {
		if (!Capabilities.PUSH_ENABLED)
			throw new IllegalAccessException("Capability PUSH disabled");
	}
	
	private static boolean isOsVersionEnough() {
		int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
		return sdkVersion >= Build.VERSION_CODES.FROYO;
	}
	
	public static void register() throws IllegalAccessException {
		if (!isOsVersionEnough())
			return;
		
		checkCapabilities();
		
		Logger.D(TAG, "Register for PUSH messages");
		
		Context ctx = RhodesService.getInstance().getContext();
		Intent registrationIntent = new Intent(C2DM_INTENT_PREFIX + "REGISTER");
		registrationIntent.putExtra("app", PendingIntent.getBroadcast(ctx, 0, new Intent(), 0));
		registrationIntent.putExtra("sender", Push.SENDER);
		ctx.startService(registrationIntent);
	}
	
	public static void unregister() throws IllegalAccessException {
		if (!isOsVersionEnough())
			return;
		
		checkCapabilities();
		
		Logger.D(TAG, "Unregister from PUSH messages");
		
		Context ctx = RhodesService.getInstance().getContext();
		Intent unregIntent = new Intent(C2DM_INTENT_PREFIX + "UNREGISTER");
		unregIntent.putExtra("app", PendingIntent.getBroadcast(ctx, 0, new Intent(), 0));
		ctx.startService(unregIntent);
	}
	
}

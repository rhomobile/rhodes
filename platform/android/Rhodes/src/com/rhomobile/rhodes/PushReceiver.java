package com.rhomobile.rhodes;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class PushReceiver extends BroadcastReceiver {
	
	private static final String TAG = PushReceiver.class.getSimpleName();
	
	private static final String REG_ID = "registration_id";
	
	public static final String INTENT_SOURCE = PushReceiver.class.getName();
	
	private static final String INTENT_PREFIX = PushReceiver.class.getPackage().getName();
	
	public static final String INTENT_TYPE = INTENT_PREFIX + ".type";
	public static final String INTENT_REGISTRATION_ID = INTENT_PREFIX + ".registration_id";
	public static final String INTENT_EXTRAS = INTENT_PREFIX + ".extras";
	
	public static final int INTENT_TYPE_UNKNOWN = 0;
	public static final int INTENT_TYPE_REGISTRATION_ID = 1;
	public static final int INTENT_TYPE_MESSAGE = 2;

	private void handleRegistration(Context context, Intent intent) {
		String id = intent.getStringExtra(REG_ID);
		String error = intent.getStringExtra("error");
		String unregistered = intent.getStringExtra("unregistered");
		if (error != null) {
			Log.d(TAG, "Received error: " + error);
		}
		else if (unregistered != null) {
			Log.d(TAG, "Unregistered: " + unregistered);
		}
		else if (id != null) {
			Intent serviceIntent = new Intent(context, RhodesService.class);
			serviceIntent.putExtra(RhodesService.INTENT_SOURCE, INTENT_SOURCE);
			serviceIntent.putExtra(INTENT_TYPE, INTENT_TYPE_REGISTRATION_ID);
			serviceIntent.putExtra(INTENT_REGISTRATION_ID, id);
			context.startService(serviceIntent);
		}
		else
			Log.w(TAG, "Unknown registration event");
	}
	
	private void handleMessage(Context context, Intent intent) {
		Intent serviceIntent = new Intent(context, RhodesService.class);
		serviceIntent.putExtra(RhodesService.INTENT_SOURCE, INTENT_SOURCE);
		serviceIntent.putExtra(INTENT_TYPE, INTENT_TYPE_MESSAGE);
		serviceIntent.putExtra(INTENT_EXTRAS, intent.getExtras());
		context.startService(serviceIntent);
	}
	
	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (action.equals(PushService.C2DM_INTENT_PREFIX + "REGISTRATION")) {
			try {
				handleRegistration(context, intent);
			}
			catch (Exception e) {
				Log.e(TAG, "Can't handle PUSH registration: " + e.getMessage());
			}
		}
		else if (action.equals(PushService.C2DM_INTENT_PREFIX + "RECEIVE")) {
			try {
				handleMessage(context, intent);
			}
			catch (Exception e) {
				Log.e(TAG, "Can't handle PUSH message: " + e.getMessage());
			}
		}
		else
			Log.w(TAG, "Unknown action received (PUSH): " + action);
		setResult(Activity.RESULT_OK, null /* data */, null /* extra */);
	}

}

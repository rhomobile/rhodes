package com.rhomobile.rhodes;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class PushReceiver extends BroadcastReceiver {
	
	private static final String TAG = "PushReceiver";
	
	private static final String REG_ID = "registration_id";

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
			// TODO: store it in reg_id variable
			Log.d(TAG, "Registered: id: " + id);
			RhodesService r = RhodesService.getInstance();
			if (r == null) {
				Log.e(TAG, "Rhodes instance is null");
				return;
			}
			r.setPushRegistrationId(id);
		}
		else
			Log.w(TAG, "Unknown registration event");
	}
	
	private void handleMessage(Context context, Intent intent) {
		RhodesService r = RhodesService.getInstance();
		if (r == null) {
			Log.e(TAG, "Rhodes instance is null");
			return;
		}
		r.handlePushMessage(intent);
	}
	
	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (action.equals(PushService.C2DM_INTENT_PREFIX + "REGISTRATION"))
			handleRegistration(context, intent);
		else if (action.equals(PushService.C2DM_INTENT_PREFIX + "RECEIVE"))
			handleMessage(context, intent);
		else
			Log.w(TAG, "Unknown action received (PUSH): " + action);
	}

}

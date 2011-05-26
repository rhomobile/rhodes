package com.rhomobile.nfc;

import java.util.Iterator;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.Log;

import com.rhomobile.rhodes.PushService;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesActivityListener;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;

public class NfcActivity  extends Activity {

	
	private static final String TAG = NfcActivity.class.getSimpleName();
	
	private static Intent ourIntent = null;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		String action = intent.getAction();
		Utils.platformLog(TAG, " NfcActivity onCreate !!! Action = "+action);
		processNewIntent(intent);
		finish();
	}

	@Override
	protected void onNewIntent(Intent intent) {
	    super.onNewIntent(intent);
		String action = intent.getAction();
		Utils.platformLog(TAG, " NfcActivity onNewIntent !!! Action = "+action);
	    processNewIntent(intent);
	}	
	
	private void processNewIntent(Intent intent) {
	    super.onNewIntent(intent);
		String action = intent.getAction();
		
		Utils.platformLog(TAG, " NfcActivity processNewIntent !!! Action = "+action);
		
		
		if ( (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) ||
		(NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action))  ||
		(NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)) ) {

			Nfc.log("NfcActivity: RhodesActivity is not Foreground - save messages and start Activity");

			Nfc.setApplicationStartupIntent(intent);
			
	        Intent run_intent = new Intent();
	        run_intent.setClass(this, RhodesActivity.class);
	     	startActivity(run_intent);
		     	
		}
	}
	
	
	public static boolean isRhodesActivityOnForeground() {
		RhodesActivity rhodes_activity = null;
		// check RhodesActivity state
		try {
			rhodes_activity = RhodesActivity.safeGetInstance();
		}
		catch(NullPointerException e) {
			
		}
		boolean isForeground = false;
		if (rhodes_activity != null) {
			isForeground = rhodes_activity.isForegroundNow();
		}
		return isForeground;
	}
	

}

package com.rhomobile.nfc;

import android.app.Activity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.Utils;

public class NfcActivity  extends Activity {

	
	private static final String TAG = NfcActivity.class.getSimpleName();
	
	private static Intent ourIntent = null;
	
	private static int getSDKVersion() {
		return Build.VERSION.SDK_INT;
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onCreate !!! START ");
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		String action = intent.getAction();
		Utils.platformLog(TAG, " NfcActivity onCreate !!! Action = "+action);
		processNewIntent(intent);
		finish();
	}

	
	@Override
	public void onStart() {
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onStart !!! START ");
		super.onStart();
	}
	
	@Override
	public void onResume() {
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onResume !!! START ");
 		super.onResume();
 		//finish();
 	}

    @Override
    public void onPause() 
    {
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onPause !!! START ");
    	super.onPause();
    }

    @Override
	public void onStop() 
	{
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onStop !!! START ");
		super.onStop();
	}
	
	@Override
	public void onDestroy() {
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onDestroy !!! START ");
		super.onDestroy();
	}
	
	
	
	
	
	@Override
	protected void onNewIntent(Intent intent) {
		Utils.platformLog(TAG, " $$$$$$$  NfcActivity onNewIntent !!! START ");
	    super.onNewIntent(intent);
		String action = intent.getAction();
		Utils.platformLog(TAG, " NfcActivity onNewIntent !!! Action = "+action);
	    processNewIntent(intent);
	}	
	
	private void processNewIntent(Intent intent) {
		/*
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
		*/
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

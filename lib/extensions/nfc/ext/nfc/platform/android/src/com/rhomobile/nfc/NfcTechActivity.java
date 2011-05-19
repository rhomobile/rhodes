package com.rhomobile.nfc;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
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
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;

public class NfcTechActivity  extends Activity {

	
	private static final String TAG = NfcTechActivity.class.getSimpleName();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
	
	/* NOT USED NOW !	
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		String action = intent.getAction();
		
		if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)) {
			Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
				Nfc.getInstance().onReceiveTag(tag);
			}
        }
		finish();
	*/
	}	
	

}

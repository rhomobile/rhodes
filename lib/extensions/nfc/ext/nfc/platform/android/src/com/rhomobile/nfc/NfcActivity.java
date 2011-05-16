package com.rhomobile.nfc;

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
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;

public class NfcActivity  extends Activity {

	
	private static final String TAG = NfcActivity.class.getSimpleName();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Intent intent = getIntent();
		String action = intent.getAction();
		
		Utils.platformLog(TAG, " $$$$$$$$$ NfcActivity STARTED !!! Action = "+action);
		
		
		if (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) {
			Utils.platformLog(TAG, "ACTION_TAG_DISCOVERED !");
            Parcelable[] rawMsgs = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
            NdefMessage[] msgs;
            if (rawMsgs != null) {
                msgs = new NdefMessage[rawMsgs.length];
                for (int i = 0; i < rawMsgs.length; i++) {
                    msgs[i] = (NdefMessage) rawMsgs[i];
                }
            } else {
                // Unknown tag type
                byte[] empty = new byte[] {};
                NdefRecord record = new NdefRecord(NdefRecord.TNF_UNKNOWN, empty, empty, empty);
                NdefMessage msg = new NdefMessage(new NdefRecord[] {record});
                msgs = new NdefMessage[] {msg};
            }
            Nfc.getInstance().onReceiveMessages(msgs);
        }
		if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
			Utils.platformLog(TAG, "ACTION_NDEF_DISCOVERED !");
			Uri u = intent.getData();
			
			Utils.platformLog(TAG, "     Data = "+u.toString());
			String t = intent.getType();
			if (t != null) {
				Utils.platformLog(TAG, "     Type = "+t);
			}
			else {
				Utils.platformLog(TAG, "     Type is NULL= ");
			}
			
			Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
				Utils.platformLog(TAG, "     Tag found in extars ! ");
			}
			else {
				Utils.platformLog(TAG, "     Tag not found in extars ! ");
			}
			
			
            Parcelable[] rawMsgs = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
            NdefMessage[] msgs;
            if (rawMsgs != null) {
                msgs = new NdefMessage[rawMsgs.length];
                for (int i = 0; i < rawMsgs.length; i++) {
                    msgs[i] = (NdefMessage) rawMsgs[i];
                }
            } else {
                // Unknown tag type
    			Utils.platformLog(TAG, "     Unknown Tag type !!!");
                byte[] empty = new byte[] {};
                NdefRecord record = new NdefRecord(NdefRecord.TNF_UNKNOWN, empty, empty, empty);
                NdefMessage msg = new NdefMessage(new NdefRecord[] {record});
                msgs = new NdefMessage[] {msg};
            }
            Nfc.getInstance().onReceiveMessages(msgs);
        }
		finish();
	}
	

}

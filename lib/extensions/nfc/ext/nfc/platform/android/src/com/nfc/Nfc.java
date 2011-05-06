package com.nfc;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.Log;

import com.rhomobile.rhodes.PushService;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;

public class Nfc  extends BroadcastReceiver {

	
	private static final String TAG = Nfc.class.getSimpleName();
	
	private static boolean ourIsEnable = false;
	private static String ourCallback = null;
	
	private static Nfc ourInstance = null;

	private static native void callCallback(String callback_url, NfcMessagePack msgpack);
	
	
	public static Nfc getInstance() {
		if (ourInstance == null) {
			ourInstance = new Nfc();
		}
		return ourInstance;
	}
	
	
	public static int isSupported() {
		NfcAdapter da = null;
		try {
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion >= Build.VERSION_CODES.GINGERBREAD_MR1) {
				da = NfcAdapter.getDefaultAdapter(RhodesService.getContext());
			}
			else if (sdkVersion >= Build.VERSION_CODES.GINGERBREAD) {
				da = NfcAdapter.getDefaultAdapter();
			}
		}
		catch (Exception e) {
			// nothing
		}
		if (da == null) {
			return 0;
		}
		int res = 0;
		if (da.isEnabled()) {
			res = 1;
		}
		return res;
	}
	
	public static int isEnabled() {
		int res = 0;
		if (ourIsEnable) {
			res = 1;
		}
		return res;
	}
	
	public static void setEnable(int enable) {
		ourIsEnable = (enable != 0);
	}
	
	public static void setCallback(String callback) {
		ourCallback = callback;
	}
	
	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		
		if (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) {
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
            onReceiveMessages(msgs);
        }
	}
	
	private static boolean isRhodesApplicationRun() {
		return RhodesService.getInstance()!=null;
	}
	
	void onReceiveMessages(NdefMessage[] msgs) {
        if (msgs == null || msgs.length == 0) {
            return;
        }
		
        Utils.platformLog(TAG, "NFC TAG Received ! Service started = "+String.valueOf(isRhodesApplicationRun()));
        
        NfcMessagePack pack = new NfcMessagePack(msgs);
        
        if (isRhodesApplicationRun()) {
        	if (ourIsEnable) {
        		if (ourCallback != null) {
        			if (ourCallback.length() > 0) {
        				callCallback(ourCallback, pack);
        			}
        		}
        	}
        }
        
        
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

}

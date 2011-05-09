package com.rhomobile.nfc;

import java.io.IOException;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.Log;

import com.rhomobile.rhodes.PushService;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;

public class Nfc  {

	
	private static final String TAG = Nfc.class.getSimpleName();
	
	private static boolean ourIsEnable = false;
	private static String ourCallback = null;
	private static String ourTechCallback = null;
	
	private static Nfc ourInstance = null;

	private static native void callCallback(String callback_url, NfcMessagePack msgpack);
	private static native void callTechCallback(String callback_url, String tech_event);
	
	private static Tag ourTag = null;
	
	
	private MifareClassic mMifareClassic = null;
	private MifareUltralight mMifareUltralight = null;
	
	private String[] mTechList = null;
	
	
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

	public static void setTechCallback(String callback) {
		ourTechCallback = callback;
	}
	
	//@Override
	//public void onReceive(Context context, Intent intent) {
	
	private static boolean isRhodesApplicationRun() {
		return RhodesService.getInstance()!=null;
	}

	
	public void onReceiveTag(Tag tag) {
		if (tag == null) {
			return;
		}
		
		ourTag = tag;
		
		int count = 0;
		mMifareClassic = MifareClassic.get(tag);
		mMifareUltralight = MifareUltralight.get(tag);
		
		if (mMifareClassic != null) {
			count++;
		}
		if (mMifareUltralight != null) {
			count++;
		}
		mTechList = new String[count];
		
		count = 0;
		if (mMifareClassic != null) {
			mTechList[count++] = "MifareClassic";
		}
		if (mMifareUltralight != null) {
			mTechList[count++] = "MifareUltralight";
		}
		
        Utils.platformLog(TAG, "NFC TAG Tech Received ! Service started = "+String.valueOf(isRhodesApplicationRun()));
        
        if (isRhodesApplicationRun()) {
        	if (ourIsEnable) {
        		if (ourTechCallback != null) {
        			if (ourTechCallback.length() > 0) {
        				callTechCallback(ourTechCallback, "discovered");
        			}
                	else {
                        Utils.platformLog(TAG, "Nfc callback is empty");
                	}
        		}
            	else {
                    Utils.platformLog(TAG, "Nfc callback is null");
            	}
        	}
        	else {
                Utils.platformLog(TAG, "Nfc is not enabled");
        	}
        }
        
	}
	
	public void onReceiveMessages(NdefMessage[] msgs) {
        if (msgs == null || msgs.length == 0) {
            return;
        }
		
        Utils.platformLog(TAG, "NFC TAG Received ! Service started = "+String.valueOf(isRhodesApplicationRun()));
        
        NfcMessagePack pack = new NfcMessagePack(msgs);
        
        Utils.platformLog(TAG, "Message Pack created");
        if (isRhodesApplicationRun()) {
        	if (ourIsEnable) {
        		if (ourCallback != null) {
        			if (ourCallback.length() > 0) {
        				callCallback(ourCallback, pack);
        			}
                	else {
                        Utils.platformLog(TAG, "Nfc callback is empty");
                	}
        		}
            	else {
                    Utils.platformLog(TAG, "Nfc callback is null");
            	}
        	}
        	else {
                Utils.platformLog(TAG, "Nfc is not enabled");
        	}
        }
        
	}
	

	public static void clear_listen_tech_list() {
		// listen all now
	}
	
	public static void add_listen_tech(String tech_name) {
		// listen all now
	}
	
	public static int get_tech_list_count() {
		return getInstance().mTechList.length;
	}
	
	public static String get_tech_list(int index) {
		return getInstance().mTechList[index];
	}

	public static void tech_connect(String tech_name) {
		if (tech_name.equals("MifareClassic")) {
			try {
				if (getInstance().mMifareClassic != null)
					getInstance().mMifareClassic.connect();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (tech_name.equals("MifareUltralight")) {
			try {
				if (getInstance().mMifareUltralight != null)
					getInstance().mMifareUltralight.connect();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public static void tech_close(String tech_name) {
		if (tech_name.equals("MifareClassic")) {
			try {
				if (getInstance().mMifareClassic != null)
					getInstance().mMifareClassic.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		if (tech_name.equals("MifareUltralight")) {
			try {
				if (getInstance().mMifareUltralight != null)
					getInstance().mMifareUltralight.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public static int tech_is_connected(String tech_name) {
		if (tech_name.equals("MifareClassic")) {
			if (getInstance().mMifareClassic != null)
				if (getInstance().mMifareClassic.isConnected() )
					return 1;
		}
		if (tech_name.equals("MifareUltralight")) {
			if (getInstance().mMifareUltralight != null)
				if (getInstance().mMifareUltralight.isConnected() ) 
					return 1;
		}
		return 0;
	}
	
	public static int tech_MifareClassic_get_size() {
		return 0;
	}
	
	public static void tech_MifareClassic_write_block(int index, byte[] block) {
		
	}
	
	public static void tech_MifareClassic_read_block(int index, byte[] block) {
		
	}
	

	public static int tech_MifareUltralight_get_size() {
		return 0;
	}
	
	public static void tech_MifareUltralight_write_page(int index, byte[] page) {
		
	}
	
	public static void tech_MifareUltralight_read_pages(int index, byte[] pages) {
		
	}
	
	
	
	

}

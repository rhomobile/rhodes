package com.rhomobile.nfc;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.Uri;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.NfcManager;
import android.nfc.Tag;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.TagTechnology;
import android.os.Build;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.Log;

import com.rhomobile.rhodes.PushService;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesActivityListener;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class Nfc implements RhodesActivityListener {

	private static final String TECH_MIFARE_CLASSIC = "MifareClassic";
	private static final String TECH_MIFARE_ULTRALIGHT = "MifareUltralight";
	private static final String TECH_NDEF = "Ndef";
	
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
	private ArrayList<String> mTechList = null;
	private Hashtable<String,TagTechnology> mTechs = null;
	
	private static final boolean isLogging = false;
	
	private static native void logNative(String msg);

	private static void log(String msg) {
		//logNative(msg);
		if (isLogging) { 
			Utils.platformLog(TAG, msg);
		}
	}
	
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
		log(" $$$$$$$$$ setEnable() START() ");
		boolean oldEnable = ourIsEnable;
		ourIsEnable = (enable != 0);
		if ((oldEnable == false) && (ourIsEnable == true)) {
			RhodesActivity.safeGetInstance().addRhodesActivityListener(getInstance());
			if (RhodesActivity.safeGetInstance().isForegroundNow()) {
				PerformOnUiThread.exec( new Runnable() {
					public void run() {
						if (RhodesActivity.safeGetInstance().isForegroundNow()) {
							getInstance().onResume(RhodesActivity.safeGetInstance());
						}
					}
				});
			}
		}
		if ((oldEnable == true) && (ourIsEnable == false)) {
			RhodesActivity.safeGetInstance().removeRhodesActivityListener(getInstance());
			if (RhodesActivity.safeGetInstance().isForegroundNow()) {
				PerformOnUiThread.exec( new Runnable() {
					public void run() {
						NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(RhodesActivity.getContext());
						nfcAdapter.disableForegroundDispatch(RhodesActivity.safeGetInstance());
					}
				});
			}
		}
		log(" $$$$$$$$$ setEnable() FINISH() ");
	}
	
	public void onPause(RhodesActivity activity) {
		log(" $$$$$$$$$ onPause() ");
		NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(RhodesActivity.getContext());
		nfcAdapter.disableForegroundDispatch(activity);
	}
	
	public void onResume(RhodesActivity activity) {
		log(" $$$$$$$$$ onResume() ");
		NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(RhodesActivity.getContext());
		IntentFilter[] filters = new IntentFilter[1];
		filters[0] = new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED);
		
		PendingIntent intent = 
            PendingIntent.getActivity(activity, 0,
              new Intent(activity, activity.getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 
              0);
		
		nfcAdapter.enableForegroundDispatch(activity, intent, null, null);
	}
	
	public void onNewIntent(RhodesActivity activity, Intent intent) {
		String action = intent.getAction();
		
		log(" $$$$$$$$$ onNewIntent !!! Action = "+action);
		
		
		if (NfcAdapter.ACTION_TAG_DISCOVERED.equals(action)) {
			log("ACTION_TAG_DISCOVERED !");
            Parcelable[] rawMsgs = intent.getParcelableArrayExtra(NfcAdapter.EXTRA_NDEF_MESSAGES);
            NdefMessage[] msgs;
            if (rawMsgs != null) {
    			log(" Found rawMessages from Intent !");
                msgs = new NdefMessage[rawMsgs.length];
                for (int i = 0; i < rawMsgs.length; i++) {
                    msgs[i] = (NdefMessage) rawMsgs[i];
                }
            } else {
                // Unknown tag type
    			log(" Intent not have any Messages !");
                byte[] empty = new byte[] {};
                NdefRecord record = new NdefRecord(NdefRecord.TNF_UNKNOWN, empty, empty, empty);
                NdefMessage msg = new NdefMessage(new NdefRecord[] {record});
                msgs = new NdefMessage[] {msg};
            }
            getInstance().onReceiveMessages(msgs);
			Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
				log("     Tag found in extars ! ");
            	onReceiveTag(tag);
			}
			else {
				log("     Tag not found in extars ! ");
			}
        }
		if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
			log("ACTION_NDEF_DISCOVERED !");
			Uri u = intent.getData();
			
			log("     Data = "+u.toString());
			String t = intent.getType();
			if (t != null) {
				log("     Type = "+t);
			}
			else {
				log("     Type is NULL= ");
			}
			
			Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
				log("     Tag found in extars ! ");
			}
			else {
				log("     Tag not found in extars ! ");
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
    			log("     Unknown Tag type !!!");
                byte[] empty = new byte[] {};
                NdefRecord record = new NdefRecord(NdefRecord.TNF_UNKNOWN, empty, empty, empty);
                NdefMessage msg = new NdefMessage(new NdefRecord[] {record});
                msgs = new NdefMessage[] {msg};
            }
            getInstance().onReceiveMessages(msgs);
            
            if (tag != null) {
            	onReceiveTag(tag);
            }
        }
		if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)) {
			log("ACTION_TECH_DISCOVERED !");
			Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
				Nfc.getInstance().onReceiveTag(tag);
			}
        }
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
		log("onReceiveTag()");
		if (tag == null) {
			return;
		}
		
		ourTag = tag;
		
		mTechs = new Hashtable<String,TagTechnology>();
		mTechList = new ArrayList<String>();
		
		mMifareClassic = MifareClassic.get(tag);
		mMifareUltralight = MifareUltralight.get(tag);

		if (mMifareClassic != null) {
			mTechs.put("MifareClassic", mMifareClassic);
			log("     MifareClassic found !");
			//try {
				//mMifareClassic.connect();
			//} catch (IOException e) {
			//	log("     MifareClassic.connect() throw Exception !!!");
			//	e.printStackTrace();
			//}
			boolean ic = mMifareClassic.isConnected();
			log("     MifareClassic.isConnected() = "+String.valueOf(ic));
			log("     MifareClassic.block_count() = "+String.valueOf(mMifareClassic.getBlockCount()));
			log("     MifareClassic.sector_count() = "+String.valueOf(mMifareClassic.getSectorCount()));
			log("     MifareClassic.size() = "+String.valueOf(mMifareClassic.getSize()));
			
			if (false) {
				int i;
				for (i = 0; i < mMifareClassic.getSectorCount(); i++) {
					try {
						boolean auth = mMifareClassic.authenticateSectorWithKeyA(i, mMifareClassic.KEY_DEFAULT);
	
						log("  sector ["+String.valueOf(i)+"]   auth = "+String.valueOf(auth));
	
						if ( !auth) {
							auth = mMifareClassic.authenticateSectorWithKeyA(i, mMifareClassic.KEY_MIFARE_APPLICATION_DIRECTORY);
							log("  sector ["+String.valueOf(i)+"]   auth with MIFARE_APP_DIR = "+String.valueOf(auth));
						}
	
						if ( !auth) {
							auth = mMifareClassic.authenticateSectorWithKeyA(i, mMifareClassic.KEY_NFC_FORUM);
							log("  sector ["+String.valueOf(i)+"]   auth with NFC = "+String.valueOf(auth));
						}
						
						if (auth) {
							int j;
							byte block[] = new byte[16];
							for (j = 0; j < mMifareClassic.getBlockCountInSector(i); j++) {
								int index = mMifareClassic.sectorToBlock(i);
								tech_MifareClassic_read_block(index + j, block);
							}
						}
					} catch (IOException e) {
						log("mMifareClassic.authenticateSectorWithKeyA() throw Exception");
						e.printStackTrace();
					}
				}
			}
			
			//try {
				//mMifareClassic.authenticateSectorWithKeyA(0, mMifareClassic.KEY_MIFARE_APPLICATION_DIRECTORY);
			//} catch (IOException e) {
			//	// TODO Auto-generated catch block
			//	e.printStackTrace();
			//}
			
		}
		if (mMifareUltralight != null) {
			mTechs.put("MifareUltralight", mMifareClassic);
		}
		

		Set<String> keys = mTechs.keySet();
		
		Iterator<String> iterator = keys.iterator();
		
		while (iterator.hasNext()) {
			String key = iterator.next();
			mTechList.add(key);
		}
		
        log("NFC TAG Tech Received ! Service started = "+String.valueOf(isRhodesApplicationRun()));
        
        if (isRhodesApplicationRun()) {
        	if (ourIsEnable) {
        		if (ourTechCallback != null) {
        			if (ourTechCallback.length() > 0) {
        				callTechCallback(ourTechCallback, "discovered");
        			}
                	else {
                        log("Nfc callback is empty");
                	}
        		}
            	else {
                    log("Nfc callback is null");
            	}
        	}
        	else {
                log("Nfc is not enabled");
        	}
        }
        
	}
	
	public void onReceiveMessages(NdefMessage[] msgs) {
		
		//return;
		
		
		log("onReceiveMessages()");
        if (msgs == null || msgs.length == 0) {
            return;
        }
		
        log("NFC TAG(with NDE Messages) Received ! Service started = "+String.valueOf(isRhodesApplicationRun()));
        
        NfcMessagePack pack = new NfcMessagePack(msgs);
        
        log("Message Pack created");
        if (isRhodesApplicationRun()) {
        	if (ourIsEnable) {
        		if (ourCallback != null) {
        			if (ourCallback.length() > 0) {
        				callCallback(ourCallback, pack);
        			}
                	else {
                        log("Nfc callback is empty");
                	}
        		}
            	else {
                    log("Nfc callback is null");
            	}
        	}
        	else {
                log("Nfc is not enabled");
        	}
        }
        //*/
	}
	

	public static void clear_listen_tech_list() {
		// listen all now
	}
	
	public static void add_listen_tech(String tech_name) {
		// listen all now
	}
	
	public static int get_tech_list_count() {
		return getInstance().mTechList.size();
	}
	
	public static String get_tech_list(int index) {
		return getInstance().mTechList.get(index);
	}
	

	public static void tech_connect(String tech_name) {
		log("tech_connect("+tech_name+") START");
		if (getInstance().mTechs == null) {
			log("     tech list is empty");
			return;
		}
		TagTechnology tech = getInstance().mTechs.get(tech_name);
		if (tech == null) {
			log("     tech is not supported by current Tag");
			return;
		}
		try {
			tech.connect();
		} catch (IOException e) {
			log("Exception in tech_connect()");
			e.printStackTrace();
		}
		log("tech_connect("+tech_name+") FINISH");
	}
	
	public static void tech_close(String tech_name) {
		if (getInstance().mTechs == null) {
			return;
		}
		TagTechnology tech = getInstance().mTechs.get(tech_name);
		if (tech == null) {
			return;
		}
		try {
			tech.close();
		} catch (IOException e) {
			log("Exception in tech_close()");
			e.printStackTrace();
		}
	}
	
	public static int tech_is_connected(String tech_name) {
		if (getInstance().mTechs == null) {
			log ("tech_is_connected("+tech_name+") return FALSE because tech list is empty");
			return 0;
		}
		TagTechnology tech = getInstance().mTechs.get(tech_name);
		if (tech == null) {
			log ("tech_is_connected("+tech_name+") return FALSE because tech is not supported");
			return 0;
		}
		if (tech.isConnected()) {
			log ("tech_is_connected("+tech_name+") return TRUE");
			return 1;
		}
		log ("tech_is_connected("+tech_name+") return FALSE");
		return 0;
	}
	
	public static int tech_MifareClassic_get_size() {
		if (getInstance().mMifareClassic != null)
			return getInstance().mMifareClassic.getSize();
		return 0;
	}
	
	public static int tech_MifareClassic_get_block_count() {
		if (getInstance().mMifareClassic != null)
			return getInstance().mMifareClassic.getBlockCount();
		return 0;
	}

	public static int tech_MifareClassic_get_sector_count() {
		if (getInstance().mMifareClassic != null)
			return getInstance().mMifareClassic.getSectorCount();
		return 0;
	}

	public static int tech_MifareClassic_get_blocks_in_sector_count(int index) {
		if (getInstance().mMifareClassic != null)
			return getInstance().mMifareClassic.getBlockCountInSector(index);
		return 0;
	}

	public static int tech_MifareClassic_sector_to_block(int index) {
		if (getInstance().mMifareClassic != null)
			return getInstance().mMifareClassic.sectorToBlock(index);
		return 0;
	}

	public static int tech_MifareClassic_authenticate_sector_with_key_A(int index, byte[] key) {
		if (getInstance().mMifareClassic != null) {
			try {
				boolean auth = getInstance().mMifareClassic.authenticateSectorWithKeyA(index, key);
				if (auth) {
					return 1;
				}
			}
			catch (IOException e) {
				log("IOException in tech_MifareClassic_authenticate_sector_with_key_A()");
			}
		}
		return 0;
	}
	
	public static int tech_MifareClassic_authenticate_sector_with_key_B(int index, byte[] key) {
		if (getInstance().mMifareClassic != null) {
			try {
				boolean auth = getInstance().mMifareClassic.authenticateSectorWithKeyB(index, key);
				if (auth) {
					return 1;
				}
			}
			catch (IOException e) {
				log("IOException in tech_MifareClassic_authenticate_sector_with_key_B()");
			}
		}
		return 0;
	}
	
	
	
	
	
	
	public static void tech_MifareClassic_write_block(int index, byte[] block) {
		
	}
	
	public static int tech_MifareClassic_read_block(int index, byte[] block) {
		log ("tech_MifareClassic_read_block("+String.valueOf(index)+") START");
		
		int i;
		for (i = 0; i < 16; i++) {
			block[i] = 0;
		}

		if (getInstance().mMifareClassic == null) {
			log ("     MifareClassic is not supported in current Tag");
			return 0;
		}
		byte[] readed_block = null;
		try {
			readed_block = getInstance().mMifareClassic.readBlock(index);
		} catch (IOException e) {
			log ("     Exception in tech_MifareClassic_read_block("+String.valueOf(index)+")");
			e.printStackTrace();
		}
		if (readed_block == null) {
			log ("     null array returned !");
			return 0;
		}
		StringBuffer s = new StringBuffer();
		s.append("   block is : ");
		for (i = 0; (i < readed_block.length) && (i < block.length); i++) {
			block[i] = readed_block[i];
			s.append(block[i]);
			s.append(":");
		}
		log(s.toString());
		log("tech_MifareClassic_read_block("+String.valueOf(index)+") FINISH");
		return 1;
	}
	
	
	public static void tech_MifareUltralight_write_page(int index, byte[] page) {
		
	}
	
	public static void tech_MifareUltralight_read_pages(int index, byte[] pages) {
		
	}
	
	
	
	

}

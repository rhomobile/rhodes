package com.rhomobile.nfc;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Set;

import android.app.Dialog;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.Uri;
import android.nfc.FormatException;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.nfc.tech.IsoDep;
import android.nfc.tech.MifareClassic;
import android.nfc.tech.MifareUltralight;
import android.nfc.tech.Ndef;
import android.nfc.tech.NdefFormatable;
import android.nfc.tech.NfcA;
import android.nfc.tech.NfcB;
import android.nfc.tech.NfcF;
import android.nfc.tech.NfcV;
import android.nfc.tech.TagTechnology;
import android.os.Build;
import android.os.Parcelable;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoListener;
import com.rhomobile.rhodes.extmanager.AbstractRhoListener;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;
import android.app.Activity;


public class Nfc extends AbstractRhoListener implements IRhoListener {

	private static final String TAG = Nfc.class.getSimpleName();
	
	private static boolean ourIsEnable = false;
	private static String ourCallback = null;
	private static String ourTechCallback = null;
	
	private static NdefMessage ourP2PNdefMessage = null;
	
	private static Nfc ourInstance = null;

	private static native void callCallback(String callback_url, NfcMessagePack msgpack);
	private static native void callTechCallback(String callback_url, String tech_event);
	
	private static Tag ourTag = null;
	
	private MifareClassic mMifareClassic = null;
	private MifareUltralight mMifareUltralight = null;
	private Ndef mNdef = null;
	private NfcA mNfcA = null;
	private IsoDep mIsoDep = null;
	private NdefFormatable mNdefFormatable = null;
	private NfcB mNfcB = null;
	private NfcF mNfcF = null;
	private NfcV mNfcV = null;
	private ArrayList<String> mTechList = null;
	private Hashtable<String,TagTechnology> mTechs = null;
	
	private static final boolean isLogging = false;
	
	private static native void logNative(String msg);
	
	
	private static NdefMessage[] ourApplicationStartupMessages = null;
	private static Tag ourApplicationStartupTag = null;
	
	
	public static void setApplicationStartupIntent(Intent intent) {
		getInstance().onNewIntent(null, intent, true);
	}
	
	public static void performOpenApplicationTag() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				if (ourApplicationStartupMessages != null) {
					getInstance().onReceiveMessages(ourApplicationStartupMessages);
					ourApplicationStartupMessages = null;
				}
				else {
					log("ourApplicationStartupMessages is NULL !");
				}
				if (ourApplicationStartupTag != null) {
					getInstance().onReceiveTag(ourApplicationStartupTag);
					ourApplicationStartupTag = null;
				}
				else {
					log("ourApplicationStartupTag is NULL !");
				}
			}
		}, 100);
	}
	

	public static void log(String msg) {
		//logNative(msg);
		if (isLogging) { 
			Utils.platformLog(TAG, msg);
		}
	}

	public static void loge(String msg) {
		Utils.platformLog(TAG, msg);
	}
	
	public static Nfc getInstance() {
		return ourInstance;
	}
	
	private static NfcAdapter getDefaultAdapter(Context ctx) {
		NfcAdapter da = null;
		try {
			int sdkVersion = Build.VERSION.SDK_INT;
			if (sdkVersion >= Build.VERSION_CODES.GINGERBREAD_MR1) {
				da = NfcAdapter.getDefaultAdapter(ctx);
			} 
		}
		catch (Exception e) {
			// nothing
			Utils.platformLog(TAG, "Exception during get NFCAdapter: " + e.getMessage());
			e.printStackTrace();
		}
		return da;
	}
	
	public static int isSupported() {
		NfcAdapter da = getDefaultAdapter(ContextFactory.getContext());
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
			//RhodesActivity.safeGetInstance().addRhodesActivityListener(getInstance());
			PerformOnUiThread.exec( new Runnable() {
				public void run() {
					if (RhodesActivity.safeGetInstance().isForegroundNow()) {
						getInstance().onResume(RhodesActivity.safeGetInstance());
					}
				}
			});
		}
		if ((oldEnable == true) && (ourIsEnable == false)) {
			//RhodesActivity.safeGetInstance().removeRhodesActivityListener(getInstance());
			PerformOnUiThread.exec( new Runnable() {
				public void run() {
					if (RhodesActivity.safeGetInstance().isForegroundNow()) {
						getInstance().onPause(RhodesActivity.safeGetInstance());
					}
				}
			});
		}
		log(" $$$$$$$$$ setEnable() FINISH() ");
	}
	
	@Override
	public void onCreate(RhodesActivity activity, Intent intent) {
		onNewIntent(activity, intent, true);
	}
	
	@Override
	public void onCreateApplication(IRhoExtManager extManager) {
	    ourInstance = this;
		extManager.addRhoListener(this);
	}
	
	@Override
	public void onPause(RhodesActivity activity) {
		log(" $$$$$$$$$ onPause() ");
		NfcAdapter nfcAdapter = getDefaultAdapter(activity);
		if (nfcAdapter != null) {
			nfcAdapter.disableForegroundDispatch(activity);

			// UNSUPPORTED FROM ANDROID 14 
			// nfcAdapter.disableForegroundNdefPush(activity);
		}
	}
	
	@Override
	public void onResume(RhodesActivity activity) {
		log(" $$$$$$$$$ onResume() ");
		NfcAdapter nfcAdapter = getDefaultAdapter(activity);
		if ((nfcAdapter != null) && (ourIsEnable)) {
			IntentFilter[] filters = new IntentFilter[1];
			filters[0] = new IntentFilter(NfcAdapter.ACTION_NDEF_DISCOVERED);
			
			int flags = PendingIntent.FLAG_IMMUTABLE;
			PendingIntent intent = 
	            PendingIntent.getActivity(activity, 0,
	              new Intent(activity, activity.getClass()).addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 
	              flags);
			
			nfcAdapter.enableForegroundDispatch(activity, intent, null, null);
			if (ourP2PNdefMessage != null) {

				// UNSUPPORTED FROM ANDROID 14 
				// nfcAdapter.enableForegroundNdefPush(activity, ourP2PNdefMessage);			
			}
		}
	}
	
	public void onNewIntent(RhodesActivity activity, Intent intent, boolean postpone) {
		String action = intent.getAction();
		
		Utils.platformLog("Nfc", "onNewIntent !!! Action = "+action+"     postpone="+String.valueOf(postpone));
		
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
            if (postpone) {
            	ourApplicationStartupMessages = msgs;
            }
            else {
            	getInstance().onReceiveMessages(msgs);
            }
            Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
				log("     Tag found in extars ! ");
	            if (postpone) {
	            	ourApplicationStartupTag = tag;
	            }
	            else {
	            	getInstance().onReceiveTag(tag);
	            }
			}
			else {
				log("     Tag not found in extars ! ");
			}
        }
		if (NfcAdapter.ACTION_NDEF_DISCOVERED.equals(action)) {
			log("ACTION_NDEF_DISCOVERED !");
			Uri u = intent.getData();
			
			if (u != null) {
				log("     Data = "+u.toString());
			}
			else {
				log("     Data = NULL");
			}
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
            if (postpone) {
            	ourApplicationStartupMessages = msgs;
            }
            else {
            	getInstance().onReceiveMessages(msgs);
            }
            
            if (tag != null) {
	            if (postpone) {
	            	ourApplicationStartupTag = tag;
	            }
	            else {
	            	getInstance().onReceiveTag(tag);
	            }
            }
        }
		if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(action)) {
			log("ACTION_TECH_DISCOVERED !");
			Tag tag = (Tag)intent.getExtras().get(NfcAdapter.EXTRA_TAG);
			if (tag != null) {
	            if (postpone) {
	            	ourApplicationStartupTag = tag;
	            }
	            else {
	            	getInstance().onReceiveTag(tag);
	            }
			}
        }
	}
	
    @Override
	public void onNewIntent(RhodesActivity activity, Intent intent) {
		if (activity.isInsideStartStop()) {
			onNewIntent(activity, intent, false);
		}
		else {
			onNewIntent(activity, intent, true);
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
		//if (!ourIsEnable) {
		//	return;
		//}
		
		log("onReceiveTag()");
		if (tag == null) {
			return;
		}
		
		ourTag = tag;
		
		mTechs = new Hashtable<String,TagTechnology>();
		mTechList = new ArrayList<String>();
		
		mMifareClassic = MifareClassic.get(tag);
		mMifareUltralight = MifareUltralight.get(tag);
		mNdef = Ndef.get(tag);
		mNfcA = NfcA.get(tag);
		mIsoDep = IsoDep.get(tag);
		mNdefFormatable = NdefFormatable.get(tag);
		mNfcB = NfcB.get(tag);
		mNfcF = NfcF.get(tag);
		mNfcV = NfcV.get(tag);

		if (mMifareClassic != null) {
			mTechs.put("MifareClassic", mMifareClassic);
			if (isLogging) {
				log("     MifareClassic found !");
				log("     MifareClassic.block_count() = "+String.valueOf(mMifareClassic.getBlockCount()));
				log("     MifareClassic.sector_count() = "+String.valueOf(mMifareClassic.getSectorCount()));
				log("     MifareClassic.size() = "+String.valueOf(mMifareClassic.getSize()));
			}
		}
		if (mMifareUltralight != null) {
			mTechs.put("MifareUltralight", mMifareUltralight);
		}
		if (mNdef != null) {
			mTechs.put("Ndef", mNdef);
		}
		if (mNfcA != null) {
			mTechs.put("NfcA", mNfcA);
		}
		if (mIsoDep != null) {
			mTechs.put("IsoDep", mIsoDep);
		}
		if (mNdefFormatable != null) {
			mTechs.put("NdefFormatable", mNdefFormatable);
		}
		if (mNfcB != null) {
			mTechs.put("NfcB", mNfcB);
		}
		if (mNfcF != null) {
			mTechs.put("NfcF", mNfcF);
		}
		if (mNfcV != null) {
			mTechs.put("NfcV", mNfcV);
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
        				if (mTechList.size() > 0) {
        					log("call Tech callback !");
        					callTechCallback(ourTechCallback, "discovered");
        				}
        				else {
                            log("received Tag is not support any techs - empty Tag !, then no callback !");
        				}
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
        else {
            log("Rhodes application is not runned");
        }
        
	}
	
	public void onReceiveMessages(NdefMessage[] msgs) {
		//if (!ourIsEnable) {
		//	return;
		//}
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
			loge("Exception in tech_connect()");
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
			loge("Exception in tech_close()");
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
	
	public static int tech_MifareClassic_get_type() {
		if (getInstance().mMifareClassic != null)
			return getInstance().mMifareClassic.getType();
		return -2;
	}
	
	public static byte[] tech_MifareClassic_transceive(byte[] data) {
		log ("tech_MifareClassic_transceive() START");
		
		if (getInstance().mMifareClassic == null) {
			log ("     MifareClassic is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mMifareClassic.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_MifareClassic_transceive()");
			e.printStackTrace();
		}
		log("tech_MifareClassic_transceive() FINISH");
		return result;
	}
	
	public static byte[] tag_get_id() {
		if (ourTag == null) {
			return null;
		}
		return ourTag.getId();
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
				loge("IOException in tech_MifareClassic_authenticate_sector_with_key_A()");
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
				loge("IOException in tech_MifareClassic_authenticate_sector_with_key_B()");
			}
		}
		return 0;
	}
		
	public static void tech_MifareClassic_write_block(int index, byte[] block) {
		log ("tech_MifareClassic_write_block("+String.valueOf(index)+") START");
		
		if (getInstance().mMifareClassic == null) {
			log ("     MifareClassic is not supported in current Tag");
			return;
		}
		try {
			getInstance().mMifareClassic.writeBlock(index, block);
		} catch (IOException e) {
			loge("     Exception in tech_MifareClassic_write_block("+String.valueOf(index)+")");
			e.printStackTrace();
		}
		log("tech_MifareClassic_read_block("+String.valueOf(index)+") FINISH");
		
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
			loge("     Exception in tech_MifareClassic_read_block("+String.valueOf(index)+")");
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
	
	

	public static int tech_Ndef_get_max_size() {
		if (getInstance().mNdef != null)
			return getInstance().mNdef.getMaxSize();
		return 0;
	}

	public static String tech_Ndef_get_type() {
		if (getInstance().mNdef != null)
			return getInstance().mNdef.getType();
		return null;
	}

	public static int tech_Ndef_is_writable() {
		int res = 0;
		if (getInstance().mNdef != null)
			if (getInstance().mNdef.isWritable()) {
				res = 1;
			}
		return res;
	}

	public static int tech_Ndef_can_make_read_only() {
		int res = 0;
		if (getInstance().mNdef != null)
			if (getInstance().mNdef.canMakeReadOnly()) {
				res = 1;
			}
		return res;
	}

	public static int tech_Ndef_make_read_only() {
		int res = 0;
		if (getInstance().mNdef != null)
			try {
				if (getInstance().mNdef.makeReadOnly()) {
					res = 1;
				}
			} catch (IOException e) {
				loge("     Exception in tech_Ndef_make_read_only()");
				e.printStackTrace();
			}
		return res;
	}

	public static void tech_Ndef_write_Nde_message(byte[] message) {
		/*
		if (message != null) {
			Utils.platformLog("MSG", " ID = "+message.toString());
			
			StringBuffer s = new StringBuffer();
			s.append("   Message is ["+String.valueOf(message.length)+"]   : ");
			int i;
			for (i = 0; i < message.length; i++) {
				s.append(message[i]);
				s.append(":");
			}
			Utils.platformLog("MSG", s.toString());
			
			
		}
		else {
			Utils.platformLog("MSG", " message is NULL !!!");
		}
		*/
	    try {
			NdefMessage m = new NdefMessage(message);
			if (getInstance().mNdef != null) {
				getInstance().mNdef.writeNdefMessage(m);
			}
		} catch (FormatException e) {
			loge("Format exception in tech_Ndef_write_Nde_message()");
			e.printStackTrace();
		} catch (IOException e) {
			loge("IO exception in tech_Ndef_write_Nde_message()");
			e.printStackTrace();
		}
	}

	public static byte[] tech_Ndef_read_Nde_message() {
		NdefMessage m = null;
		if (getInstance().mNdef != null) {
			try {
				m = getInstance().mNdef.getNdefMessage();
			} catch (IOException e) {
				loge("IO exception in tech_Ndef_read_Nde_message()");
				e.printStackTrace();
			} catch (FormatException e) {
				loge("Format exception in tech_Ndef_read_Nde_message()");
				e.printStackTrace();
			}
		}
		if (m != null) {
			return m.toByteArray();
		}
		return null;
	}

	public static byte[] tech_NfcA_get_Atqa() {
		if (getInstance().mNfcA != null) {
			return getInstance().mNfcA.getAtqa();
		}
		return null;
	}

	public static int tech_NfcA_get_Sak() {
		if (getInstance().mNfcA != null) {
			return getInstance().mNfcA.getSak();
		}
		return 0;
	}

	public static byte[] tech_NfcA_transceive(byte[] data) {
		if (getInstance().mNfcA == null) {
			log ("     mNfcA is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mNfcA.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_NfcA_transceive()");
			e.printStackTrace();
		}
		return result;
	}


	public static byte[] tech_IsoDep_get_hi_layer_responce() {
		if (getInstance().mIsoDep != null) {
			return getInstance().mIsoDep.getHiLayerResponse();
		}
		return null;
	}

	public static byte[] tech_IsoDep_get_historical_bytes() {
		if (getInstance().mIsoDep != null) {
			return getInstance().mIsoDep.getHistoricalBytes();
		}
		return null;
	}

	public static void tech_IsoDep_set_timeout(int timeout) {
		if (getInstance().mIsoDep != null) {
			getInstance().mIsoDep.setTimeout(timeout);
		}
	}

	public static byte[] tech_IsoDep_transceive(byte[] data) {
		if (getInstance().mIsoDep == null) {
			log ("     IsoDep is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mIsoDep.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_mIsoDep_transceive()");
			e.printStackTrace();
		}
		return result;
	}

	public static void tech_NdefFormatable_format(byte[] ndef_message_byte_array) {
	    try {
			NdefMessage m = new NdefMessage(ndef_message_byte_array);
			if (getInstance().mNdefFormatable != null) {
				getInstance().mNdefFormatable.format(m);
			}
		} catch (FormatException e) {
			loge("Format exception in tech_NdefFormatable_format()");
			e.printStackTrace();
		} catch (IOException e) {
			loge("IO exception in tech_NdefFormatable_format()");
			e.printStackTrace();
		}
	}

	public static void tech_NdefFormatable_format_read_only(byte[] ndef_message_byte_array) {
	    try {
			NdefMessage m = new NdefMessage(ndef_message_byte_array);
			if (getInstance().mNdefFormatable != null) {
				getInstance().mNdefFormatable.formatReadOnly(m);
			}
		} catch (FormatException e) {
			loge("Format exception in tech_NdefFormatable_format_read_only()");
			e.printStackTrace();
		} catch (IOException e) {
			loge("IO exception in tech_NdefFormatable_format_read_only()");
			e.printStackTrace();
		}
	}

	public static byte[] tech_NfcB_get_application_data() {
		if (getInstance().mNfcB != null) {
			return getInstance().mNfcB.getApplicationData();
		}
		return null;
	}

	public static byte[] tech_NfcB_get_protocol_info() {
		if (getInstance().mNfcB != null) {
			return getInstance().mNfcB.getProtocolInfo();
		}
		return null;
	}

	public static byte[] tech_NfcB_transceive(byte[] data) {
		if (getInstance().mNfcB == null) {
			log ("     NfcB is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mNfcB.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_NfcB_transceive()");
			e.printStackTrace();
		}
		return result;
	}

	public static byte[] tech_NfcF_get_manufacturer() {
		if (getInstance().mNfcF != null) {
			return getInstance().mNfcF.getManufacturer();
		}
		return null;
	}

	public static byte[] tech_NfcF_get_system_code() {
		if (getInstance().mNfcF != null) {
			return getInstance().mNfcF.getSystemCode();
		}
		return null;
	}

	public static byte[] tech_NfcF_transceive(byte[] data) {
		if (getInstance().mNfcF == null) {
			log ("     NfcF is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mNfcF.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_NfcF_transceive()");
			e.printStackTrace();
		}
		return result;
	}

	public static int tech_NfcV_get_dsf_id() {
		if (getInstance().mNfcV != null) {
			return getInstance().mNfcV.getDsfId();
		}
		return 0;
	}

	public static int tech_NfcV_get_responce_flags() {
		if (getInstance().mNfcV != null) {
			return getInstance().mNfcV.getResponseFlags();
		}
		return 0;
	}

	public static byte[] tech_NfcV_transceive(byte[] data) {
		if (getInstance().mNfcV == null) {
			log ("     NfcV is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mNfcV.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_NfcV_transceive()");
			e.printStackTrace();
		}
		return result;
	}
	
	
	public static NfcRecord convert_byte_array_to_NdeRecord_hash(byte[] array) {
	    return new NfcRecord(array);
	}

	public static byte[] convert_NdeRecord_hash_to_byte_array(byte[] id, byte[] payload, int tnf, byte[] type) {
	    NfcRecord r = new NfcRecord((short)tnf, type, id, payload);
	    return r.getByteArray();
	}

	public static NfcMessage convert_NdeMessage_byte_array_to_NdeRecords_array(byte[] array) {
		return new NfcMessage(array);
	}

	public static NfcMessage make_empty_NfcMessage() {
		return new NfcMessage();
	}

	public static NfcRecord make_NfcRecord(byte[] id, byte[] payload, int tnf, byte[] type) {
		return new NfcRecord((short)tnf, type, id, payload);
	}
	
	public static String make_string_from_payload(byte[] payload, int tnf, byte[] type) {
		return NfcRecord.makePayloadString((short)tnf, type, payload, null);
	}

	public static byte[] make_payload_with_absolute_uri(String str) {
	    return str.getBytes();
	}

	public static byte[] make_payload_with_well_known_text(String language, String str) {
	    byte[] b_lang = language.getBytes();
	    byte[] b_str = str.getBytes();
	    byte[] result = new byte[b_lang.length + b_str.length+1];
	    
	    result[0] = (byte)b_lang.length;
	    int i;
	    for (i = 0; i < b_lang.length; i++) {
	    	result[1+i] = b_lang[i];
	    }
	    for (i = 0; i < b_str.length; i++) {
	    	result[1+b_lang.length+i] = b_str[i];
	    }
	    return result;
	}

	public static byte[] make_payload_with_well_known_uri(int prefix, String str) {
		byte[] b_str = str.getBytes();
	    byte[] result = new byte[1+b_str.length];
	    
	    result[0] = (byte)prefix;
	    int i;
	    for (i = 0; i < b_str.length; i++) {
	    	result[1+i] = b_str[i];
	    }
	    return result;
	}

	public static void p2p_enable_foreground_nde_push(byte[] nde_message_byte_array) {
		ourP2PNdefMessage = null;
		try {
			ourP2PNdefMessage = new NdefMessage(nde_message_byte_array);
		} catch (FormatException e) {
			loge("Format Exception during prepare NdefMessage in p2p_enable_foreground_nde_push() !");
			e.printStackTrace();
		}
		if ((ourP2PNdefMessage != null) && (ourIsEnable)) {
			if (RhodesActivity.safeGetInstance().isForegroundNow()) {
				PerformOnUiThread.exec( new Runnable() {
					public void run() {
						if (RhodesActivity.safeGetInstance().isForegroundNow()) {
							NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(RhodesActivity.getContext());
							if (nfcAdapter != null) {

								// UNSUPPORTED FROM ANDROID 14 
								// nfcAdapter.enableForegroundNdefPush(RhodesActivity.safeGetInstance(), ourP2PNdefMessage);
							}
						}
					}
				});
			}
		}
	}

	public static void p2p_disable_foreground_nde_push() {
		ourP2PNdefMessage = null;
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
				if (RhodesActivity.safeGetInstance().isForegroundNow()) {
					NfcAdapter nfcAdapter = NfcAdapter.getDefaultAdapter(RhodesActivity.getContext());
					if (nfcAdapter != null) {
						
						// UNSUPPORTED FROM ANDROID 14 
						// nfcAdapter.disableForegroundNdefPush(RhodesActivity.safeGetInstance());
					}
				}
			}
		});
	}

	public static void tech_MifareUltralight_write_page(int index, byte[] page) {
		if (getInstance().mMifareUltralight == null) {
			log ("     MifareUltralight is not supported in current Tag");
			return;
		}
		try {
			getInstance().mMifareUltralight.writePage(index, page);
		} catch (IOException e) {
			loge("     Exception in tech_MifareUltralight_write_page("+String.valueOf(index)+")");
			e.printStackTrace();
		}
	}
	
	public static byte[] tech_MifareUltralight_read_pages(int index) {
		byte[] result = null;
		if (getInstance().mMifareUltralight == null) {
			log ("     MifareUltralight is not supported in current Tag");
			return null;
		}
		try {
			result = getInstance().mMifareUltralight.readPages(index);
		} catch (IOException e) {
			loge("     Exception in tech_MifareUltralight_read_pages("+String.valueOf(index)+")");
			e.printStackTrace();
		}
		return result;
	}
	
	public static int tech_MifareUltralight_get_type() {
		if (getInstance().mMifareUltralight != null)
			return getInstance().mMifareUltralight.getType();
		return -2;
	}

	public static byte[] tech_MifareUltralight_transceive(byte[] data) {
		if (getInstance().mMifareUltralight == null) {
			log ("     MifareUltralight is not supported in current Tag");
			return null;
		}
		byte[] result = null;
		try {
			result = getInstance().mMifareUltralight.transceive(data);
		} catch (IOException e) {
			loge("     Exception in tech_MifareUltralight_transceive()");
			e.printStackTrace();
		}
		return result;
	}

}

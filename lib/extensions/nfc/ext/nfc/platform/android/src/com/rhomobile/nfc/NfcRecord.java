package com.rhomobile.nfc;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.Arrays;

import android.nfc.FormatException;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;

import com.rhomobile.rhodes.Utils;

public class NfcRecord {

	private byte[] 	mId = null;
	private byte[] 	mPayload = null;
	private String 	mPayloadString = null;
	private int 	mTnf = 0;
	private byte[] 	mType = null;
	private byte[] 	mByteArray = null;
	
	private NfcMessage mSubrecords = null;
	
	
	public NfcRecord(byte[] array) {
		NdefRecord r = null;
		try {
			r = new NdefRecord(array);
		} catch (FormatException e) {
			Nfc.loge("Exception during make NdefRecord from byte array !");
			e.printStackTrace();
		}
		initFromNdefRecord(r);
	}
	
	public NfcRecord(NdefRecord rec) {
		initFromNdefRecord(rec);
	}

	public NfcRecord(short tnf, byte[] type, byte[] id, byte[] payload) {
		NdefRecord r = null;
		r = new NdefRecord(tnf, type, id, payload);
		initFromNdefRecord(r);
	}
	
	public void initFromNdefRecord(NdefRecord rec) {

		if (rec == null) {
			return;
		}
		
		mId = rec.getId();
		
		/*
		if (mId != null) {
			Utils.platformLog("NfcRecord", " ID = "+mId.toString());
			
			StringBuffer s = new StringBuffer();
			s.append("   ID is : ");
			int i;
			for (i = 0; i < mId.length; i++) {
				s.append(mId[i]);
				s.append(":");
			}
			Utils.platformLog("NfcRecord", s.toString());
			
			
		}
		else {
			Utils.platformLog("NfcRecord", " ID is NULL !!!");
		}
		*/
		
		mPayload = rec.getPayload();
		mTnf = rec.getTnf();
		mType = rec.getType();
		mByteArray = rec.toByteArray();
		mPayloadString = "";
		
		mPayloadString = makePayloadString((short)mTnf, mType, mPayload, this);
	}
	
	public static String makePayloadString(short tnf, byte[] type, byte[] payload, NfcRecord record) {
        String payload_string = "";
		try {
			// detect type for prepare string from payload
			if (tnf == NdefRecord.TNF_ABSOLUTE_URI) {
				payload_string = new String(payload, Charset.forName("UTF-8"));
			}
			else if (tnf == NdefRecord.TNF_WELL_KNOWN) {
				if (Arrays.equals(type, NdefRecord.RTD_URI)) {
			        String prefix = getUriPrefixByNfcCode(payload[0]);
			        String uri = new String(Arrays.copyOfRange(payload, 1, payload.length), Charset.forName("UTF-8"));
			        payload_string = prefix + uri;
				}
				else if (Arrays.equals(type, NdefRecord.RTD_TEXT)) {
		            String textEncoding = ((payload[0] & 0200) == 0) ? "UTF-8" : "UTF-16";
		            int languageCodeLength = payload[0] & 0077;
		            String languageCode = new String(payload, 1, languageCodeLength, "US-ASCII");
		            String text = new String(payload, languageCodeLength + 1, payload.length - languageCodeLength - 1, textEncoding);
		            payload_string = text;
				}
				else if (Arrays.equals(type, NdefRecord.RTD_SMART_POSTER)) {
				       try {
				            NdefMessage subRecords = new NdefMessage(payload);
				            if (record != null) {
				            	record.mSubrecords = new NfcMessage(subRecords);
				            }
				       } catch (FormatException e) {
				            //throw new IllegalArgumentException(e);
				    	   Nfc.loge("constructor of NfcRecord: Invalid record with RTD_SMART_POSTER type !");
				       }
				}
				else {
					payload_string = new String(payload, Charset.forName("UTF-8"));
				}
			}
			else {
				payload_string = new String(payload, Charset.forName("UTF-8"));
			}
        } catch (UnsupportedEncodingException e) {
            // should never happen unless we get a malformed tag.
            //throw new IllegalArgumentException(e);
        	Nfc.loge("construct string from payload: catch UnsupportedEncodingException - tag maybe broken !");
        }
		
		return payload_string;
	}
	
	public NdefRecord makeNdefRecord() {
		NdefRecord r = null;
		try {
			r = new NdefRecord(getByteArray());
		} catch (FormatException e) {
			Nfc.loge("Exception during make NdefRecord from byte array !");
			e.printStackTrace();
		}
		return r;
	}
	
	public byte[] getId() {
		return mId;
	}
	
	public byte[] getPayload() {
		return mPayload;
	}
	
	public String getPayloadString() {
		return mPayloadString;
	}
	
	public int getTnf() {
		return mTnf;
	}
	
	public byte[] getType() {
		return mType;
	}

	public byte[] getByteArray() {
		return mByteArray;
	}
	
	public NfcMessage getSubrecords() {
		return mSubrecords;
	}
	
	
	private static String getUriPrefixByNfcCode(byte code) {
		switch(code) {
			case 0x00: return "";
	        case 0x01: return "http://www.";
	        case 0x02: return "https://www.";
	        case 0x03: return "http://";
	        case 0x04: return "https://";
	        case 0x05: return "tel:";
	        case 0x06: return "mailto:";
	        case 0x07: return "ftp://anonymous:anonymous@";
	        case 0x08: return "ftp://ftp.";
	        case 0x09: return "ftps://";
	        case 0x0A: return "sftp://";
	        case 0x0B: return "smb://";
	        case 0x0C: return "nfs://";
	        case 0x0D: return "ftp://";
	        case 0x0E: return "dav://";
	        case 0x0F: return "news:";
	        case 0x10: return "telnet://";
	        case 0x11: return "imap:";
	        case 0x12: return "rtsp://";
	        case 0x13: return "urn:";
	        case 0x14: return "pop:";
	        case 0x15: return "sip:";
	        case 0x16: return "sips:";
	        case 0x17: return "tftp:";
	        case 0x18: return "btspp://";
	        case 0x19: return "btl2cap://";
	        case 0x1A: return "btgoep://";
	        case 0x1B: return "tcpobex://";
	        case 0x1C: return "irdaobex://";
	        case 0x1D: return "file://";
	        case 0x1E: return "urn:epc:id:";
	        case 0x1F: return "urn:epc:tag:";
	        case 0x20: return "urn:epc:pat:";
	        case 0x21: return "urn:epc:raw:";
	        case 0x22: return "urn:epc:";
	        case 0x23: return "urn:nfc:";
		}
		return "";
	}
	

}

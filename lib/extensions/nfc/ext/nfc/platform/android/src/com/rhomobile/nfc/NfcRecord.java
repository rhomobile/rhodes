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
	
	
	
	public NfcRecord(NdefRecord rec) {

		mId = rec.getId();
		mPayload = rec.getPayload();
		mTnf = rec.getTnf();
		mType = rec.getType();
		mByteArray = rec.toByteArray();
		mPayloadString = "";
		
        try {
		
			// detect type for prepare string from payload
			if (mTnf == NdefRecord.TNF_ABSOLUTE_URI) {
				mPayloadString = new String(mPayload, Charset.forName("UTF-8"));
			}
			else if (mTnf == NdefRecord.TNF_WELL_KNOWN) {
				if (Arrays.equals(mType, NdefRecord.RTD_URI)) {
			        String prefix = getUriPrefixByNfcCode(mPayload[0]);
			        String uri = new String(Arrays.copyOfRange(mPayload, 1, mPayload.length), Charset.forName("UTF-8"));
			        mPayloadString = prefix + uri;
				}
				else if (Arrays.equals(mType, NdefRecord.RTD_TEXT)) {
		            String textEncoding = ((mPayload[0] & 0200) == 0) ? "UTF-8" : "UTF-16";
		            int languageCodeLength = mPayload[0] & 0077;
		            String languageCode = new String(mPayload, 1, languageCodeLength, "US-ASCII");
		            String text = new String(mPayload, languageCodeLength + 1, mPayload.length - languageCodeLength - 1, textEncoding);
					mPayloadString = text;
				}
				else if (Arrays.equals(mType, NdefRecord.RTD_SMART_POSTER)) {
				       try {
				            NdefMessage subRecords = new NdefMessage(mPayload);
				            mSubrecords = new NfcMessage(subRecords);
				       } catch (FormatException e) {
				            //throw new IllegalArgumentException(e);
				    	   Utils.platformLog("NFC support native extension", "Invalid record with RTD_SMART_POSTER type !");
				       }
				}
				else {
					mPayloadString = new String(mPayload, Charset.forName("UTF-8"));
				}
			}
			else {
				mPayloadString = new String(mPayload, Charset.forName("UTF-8"));
			}
        } catch (UnsupportedEncodingException e) {
            // should never happen unless we get a malformed tag.
            //throw new IllegalArgumentException(e);
	    	   Utils.platformLog("NFC support native extension", "catch UnsupportedEncodingException - tag maybe broken !");
        }
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
	
	
	private String getUriPrefixByNfcCode(byte code) {
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

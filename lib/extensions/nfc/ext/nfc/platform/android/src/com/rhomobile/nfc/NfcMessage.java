package com.rhomobile.nfc;

import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import com.rhomobile.rhodes.Utils;

public class NfcMessage {
	
	private NfcRecord[] mRecords = null;
	private byte[] mByteArray = null;

	public NfcMessage(NdefMessage msg) {
		mByteArray = msg.toByteArray();
		NdefRecord[] records = msg.getRecords();
		mRecords = new NfcRecord[records.length];
		int i;
		for (i = 0; i < records.length; i++) {
			mRecords[i] = new NfcRecord(records[i]);
		}
	}
	
	public int getItemCount() {
		return mRecords.length;
	}
	
	public NfcRecord getItem(int index) {
		return mRecords[index];
	}
	
	public byte[] getByteArray() {
		return mByteArray;
	}

}

package com.rhomobile.nfc;

import java.util.ArrayList;
import java.util.Iterator;

import android.nfc.FormatException;
import android.nfc.NdefMessage;
import android.nfc.NdefRecord;
import com.rhomobile.rhodes.Utils;

public class NfcMessage {
	
	private NfcRecord[] mRecords = null;
	private byte[] mByteArray = null;

	private ArrayList<NfcRecord> mConstructRecords = null;
	
	public NfcMessage(byte[] array) {
		NdefMessage r = null;
		try {
			r = new NdefMessage(array);
		} catch (FormatException e) {
			Nfc.loge("Exception during make NdefMessage from byte array !");
			e.printStackTrace();
		}
		initFromNdefMessage(r);
	}
	
	public NfcMessage(NdefMessage msg) {
		initFromNdefMessage(msg);
	}

	public NfcMessage() {
	}
	
	private void initFromNdefMessage(NdefMessage msg) {
		mByteArray = msg.toByteArray();
		NdefRecord[] records = msg.getRecords();
		mRecords = new NfcRecord[records.length];
		int i;
		for (i = 0; i < records.length; i++) {
			mRecords[i] = new NfcRecord(records[i]);
		}
	}
	
	public void addConstructRecord(NfcRecord record) {
		if (mConstructRecords == null) {
			mConstructRecords = new ArrayList<NfcRecord>();
		}
		mConstructRecords.add(record);
	}
	
	public void initFromConstructRecordList () {
		if (mConstructRecords == null) {
			return;
		}
		ArrayList<NdefRecord> records = new ArrayList<NdefRecord>();
		Iterator<NfcRecord> it = mConstructRecords.iterator();
		while (it.hasNext()) {
			NfcRecord nfcRec = it.next();
			NdefRecord ndefRec = nfcRec.makeNdefRecord();
			if (ndefRec != null) {
				records.add(ndefRec);
			}
		}
		int i;
		NdefRecord[] records_array = new NdefRecord[records.size()];
		for (i = 0; i < records.size(); i++) {
			records_array[i] = records.get(i);
		}
		NdefMessage m = new NdefMessage(records_array);
		initFromNdefMessage(m);
		mConstructRecords = null;
	}
	
	public NdefMessage makeNdefMessage() {
		NdefMessage r = null;
		try {
			r = new NdefMessage(getByteArray());
		} catch (FormatException e) {
			Nfc.loge("Exception during make NdefMessage from byte array !");
			e.printStackTrace();
		}
		return r;
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

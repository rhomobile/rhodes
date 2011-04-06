package com.nfc;

import android.nfc.NdefMessage;
import com.rhomobile.rhodes.Utils;

public class NfcMessagePack {

	
	private NfcMessage[] mMessages = null;
	
	public NfcMessagePack(NdefMessage[] msgs) {
		mMessages = new NfcMessage[msgs.length];
		int i;
		for (i = 0; i < msgs.length; i++) {
			mMessages[i] = new NfcMessage(msgs[i]);
		}
	}
	
	public int getItemCount() {
		if (mMessages == null) {
			return 0;
		}
		return mMessages.length;
	}
	
	public NfcMessage getItem(int index) {
		if (mMessages == null) {
			return null;
		}
		return mMessages[index];
	}

}

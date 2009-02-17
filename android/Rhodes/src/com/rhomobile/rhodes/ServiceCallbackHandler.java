package com.rhomobile.rhodes;

import android.os.Handler;
import android.os.Message;

public abstract class ServiceCallbackHandler extends Handler {

	/**
	 * Message handler for a input received
	 */
	private static final int MSG_RECEIVED = 0;

	/**
	 * Sends a message when an input was received by the service
	 */
	public void sendInputRecieved(int value) {

		sendMessage(obtainMessage(MSG_RECEIVED, value));
	}

	/**
	 * Removes all custom messages from the message queue
	 */
	public void removeCustomMessages() {
		removeMessages(MSG_RECEIVED);
	}

	@Override
	public void handleMessage(Message msg) {
		switch (msg.what) {
		case MSG_RECEIVED:
			Integer i = new Integer(msg.obj.toString());
			handleInputRecived(i.intValue());
		default:
			super.handleMessage(msg);
		}
	}

	/**
	 * handles when a new input is received
	 * 
	 * @param value
	 *            the new int value
	 */
	public abstract void handleInputRecived(int value);

}

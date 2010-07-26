package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.Logger;

public class ChromeClientNew extends ChromeClientOld {

	private static final String TAG = "ChromeClientNew";
	
	@Override
	public void onConsoleMessage(String message, int lineNumber, String sourceID) {
		Logger.D(TAG, message + " -- From line " + lineNumber + " of " + sourceID);
	}
	
}

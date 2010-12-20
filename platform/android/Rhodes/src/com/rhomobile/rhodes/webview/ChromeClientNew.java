package com.rhomobile.rhodes.webview;

import android.webkit.WebStorage;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;


public class ChromeClientNew extends ChromeClientOld {

	private static final String TAG = "ChromeClientNew";
	
	public ChromeClientNew(RhodesActivity rhodesActivity) {
		super(rhodesActivity);
	}

	@Override
	public void onConsoleMessage(String message, int lineNumber, String sourceID) {
		Logger.D(TAG, message + " -- From line " + lineNumber + " of " + sourceID);
	}

    public void onExceededDatabaseQuota(String url, String databaseIdentifier, long currentQuota, long estimatedSize,
            long totalUsedQuota, WebStorage.QuotaUpdater quotaUpdater) {
            quotaUpdater.updateQuota(estimatedSize * 2);
    }
	
}



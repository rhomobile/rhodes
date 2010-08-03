package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

import android.view.Window;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebView;

public class ChromeClientOld extends WebChromeClient {
	
	@Override
	public void onProgressChanged(WebView view, int newProgress) {
		if (Rhodes.ENABLE_LOADING_INDICATION) {
			newProgress *= 100;
			if (newProgress < 0)
				newProgress = 0;
			if (newProgress > Rhodes.MAX_PROGRESS)
				newProgress = Rhodes.MAX_PROGRESS;
			Rhodes r = RhodesInstance.getInstance();
			r.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, newProgress);
		}
		super.onProgressChanged(view, newProgress);
	}
	
	@Override
	public boolean onJsAlert(WebView view, String url, String message, JsResult result) {
		return false;
	}
	
	@Override
	public boolean onJsConfirm(WebView view, String url, String message, JsResult result) {
		return false;
	}
	
}

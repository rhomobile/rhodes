package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.RhodesActivity;

import android.view.Window;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebView;

public class ChromeClientOld extends WebChromeClient {
	
	private RhodesActivity mRhodesActivity;
	
	public ChromeClientOld(RhodesActivity rhodesActivity) {
		mRhodesActivity = rhodesActivity;
	}
	
	@Override
	public void onProgressChanged(WebView view, int newProgress) {
		if (RhodesActivity.ENABLE_LOADING_INDICATION) {
			newProgress *= 100;
			if (newProgress < 0)
				newProgress = 0;
			if (newProgress > RhodesActivity.MAX_PROGRESS)
				newProgress = RhodesActivity.MAX_PROGRESS;
			mRhodesActivity.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, newProgress);
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

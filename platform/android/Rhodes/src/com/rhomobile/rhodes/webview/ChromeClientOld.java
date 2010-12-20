package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhoService;

import android.view.Window;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebView;

public class ChromeClientOld extends WebChromeClient {
	
	@Override
	public void onProgressChanged(WebView view, int newProgress) {
		if (RhoService.ENABLE_LOADING_INDICATION) {
			newProgress *= 100;
			if (newProgress < 0)
				newProgress = 0;
			if (newProgress > RhoService.MAX_PROGRESS)
				newProgress = RhoService.MAX_PROGRESS;
			Rhodes r = Rhodes.getInstance();
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

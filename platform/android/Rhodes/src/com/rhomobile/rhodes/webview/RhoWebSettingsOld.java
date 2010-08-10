package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;
import android.webkit.WebView;

public class RhoWebSettingsOld implements RhoWebSettings {

	public WebSettings setWebSettings(WebView w) {
		WebSettings webSettings = w.getSettings();
		webSettings.setSavePassword(false);
		webSettings.setSaveFormData(false);
		webSettings.setJavaScriptEnabled(true);
		webSettings.setJavaScriptCanOpenWindowsAutomatically(false);
		webSettings.setSupportZoom(false);
		//webSettings.setCacheMode(WebSettings.LOAD_NO_CACHE);
		webSettings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
		webSettings.setSupportMultipleWindows(false);
		// webSettings.setLoadsImagesAutomatically(true);

		w.setVerticalScrollBarEnabled(true);
		w.setHorizontalScrollBarEnabled(true);
		w.setVerticalScrollbarOverlay(true);
		w.setHorizontalScrollbarOverlay(true);
		w.setFocusableInTouchMode(true);
		
		return webSettings;
	}

}

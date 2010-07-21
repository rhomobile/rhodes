package com.rhomobile.rhodes.webview;

import android.webkit.WebSettings;
import android.webkit.WebView;

public class RhoWebSettingsNew implements RhoWebSettings {

	public WebSettings setWebSettings(WebView w) {
		RhoWebSettingsOld ws = new RhoWebSettingsOld();
		WebSettings webSettings = ws.setWebSettings(w);
		
		webSettings.setAppCacheEnabled(true);
		
		return webSettings;
	}

}

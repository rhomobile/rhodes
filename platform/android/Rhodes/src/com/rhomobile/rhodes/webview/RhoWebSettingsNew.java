package com.rhomobile.rhodes.webview;

import android.content.Context;
import android.webkit.WebSettings;
import android.webkit.WebView;

public class RhoWebSettingsNew implements RhoWebSettings {

	public WebSettings setWebSettings(WebView w) {
		RhoWebSettingsOld ws = new RhoWebSettingsOld();
		WebSettings webSettings = ws.setWebSettings(w);
		
		webSettings.setAppCacheEnabled(true);
		
		webSettings.setDatabaseEnabled(true);
		webSettings.setJavaScriptEnabled(true);
		
		Context context = w.getContext();
		String databasePath = context.getDir("database", Context.MODE_PRIVATE).getPath();
		webSettings.setDatabasePath(databasePath);
		 
		return webSettings;
	}

}

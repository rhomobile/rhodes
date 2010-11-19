package com.rhomobile.rhodes.webview;

import com.rhomobile.rhodes.RhodesService;

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
		
		String databasePath = RhodesService.getInstance().getContext().getDir("database", Context.MODE_PRIVATE).getPath();
		webSettings.setDatabasePath(databasePath);
		 
		return webSettings;
	}

}

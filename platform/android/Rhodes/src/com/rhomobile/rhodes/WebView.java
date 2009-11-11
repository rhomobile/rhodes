package com.rhomobile.rhodes;

public class WebView {

	public static void navigate(String url) {
		RhodesInstance.getInstance().webview_navigate(url);
	}
	
	public static void refresh() {
		RhodesInstance.getInstance().webview_refresh();
	}
	
	public static String currentLocation() {
		return RhodesInstance.getInstance().webview_currentLocation();
	}
	
	public static String executeJs(String js) {
		return RhodesInstance.getInstance().webview_executeJs(js);
	}
}

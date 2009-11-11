package com.rhomobile.rhodes;

public class WebView {

	public void navigate(String url) {
		RhodesInstance.getInstance().webview_navigate(url);
	}
	
	public void refresh() {
		RhodesInstance.getInstance().webview_refresh();
	}
	
	public String currentLocation() {
		return RhodesInstance.getInstance().webview_currentLocation();
	}
	
}

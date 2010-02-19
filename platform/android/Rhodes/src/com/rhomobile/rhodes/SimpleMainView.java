package com.rhomobile.rhodes;

import android.view.View;
import android.webkit.WebView;

public class SimpleMainView implements MainView {

	private WebView webView;
	
	public View getView() {
		return webView;
	}
	
	public SimpleMainView() {
		webView = RhodesInstance.getInstance().createWebView();
	}
	
	public void back(int index) {
		webView.goBack();
	}

	public void forward(int index) {
		webView.goForward();
	}

	public void navigate(String url, int index) {
		webView.loadUrl(url);
	}
	
	public void reload(int index) {
		webView.reload();
	}
	
	public String currentLocation(int index) {
		return webView.getUrl();
	}

	public void switchTab(int index) {
		// Nothing
	}
	
	public int activeTab() {
		return 0;
	}

}

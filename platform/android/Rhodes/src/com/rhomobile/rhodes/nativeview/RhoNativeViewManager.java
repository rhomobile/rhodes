package com.rhomobile.rhodes.nativeview;


import android.view.View;
import android.webkit.WebView;
import com.rhomobile.rhodes.RhodesService;

class RhoNativeViewManager {

	public static WebView getWebViewObject(int tab_index) {
		return RhodesService.getInstance().getMainView().getWebView(tab_index);
	}

}
package com.rhomobile.rhodes.extmanager;

import android.view.View;

class RhoExtDataImpl implements IRhoExtData {
	
	View mWebView;
    private int mTabIndex;
	
	public RhoExtDataImpl(View view, int tabIndex) {
	    mWebView = view;
		mTabIndex = tabIndex;
	}
	
	public int getTabIndex() {
		return mTabIndex;
	}
	public View getWebView() {
	    return mWebView;
	}
}

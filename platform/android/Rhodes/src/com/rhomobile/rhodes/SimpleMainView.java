/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
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
		webView.setId(Rhodes.RHO_MAIN_VIEW);
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

/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.mainview;

import java.util.Map;

import com.rhomobile.rhodes.webview.WebView;

import android.view.View;

public interface MainView {

	public View getView();

	public WebView getWebView(int tab_index);
	
	public WebView detachWebView();
	
	public void back(int index);
	public void forward(int index);
	
	// Browser native 'back'
	public void goBack();
	
	public void navigate(String url, int index);
	public void reload(int index);
	
	public String currentLocation(int index);
	
	public void switchTab(int index);
	public int activeTab();
	public int getTabsCount();
	
	public void loadData(String data, int index);
	
	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right);
	public void removeNavBar();
}

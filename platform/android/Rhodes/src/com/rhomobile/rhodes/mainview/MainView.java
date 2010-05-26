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
package com.rhomobile.rhodes.mainview;

import java.util.Map;

import android.view.View;
import android.webkit.WebView;

public interface MainView {

	public View getView();
	
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
	
	public void loadData(String data, int index);
	
	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right);
	public void removeNavBar();
}

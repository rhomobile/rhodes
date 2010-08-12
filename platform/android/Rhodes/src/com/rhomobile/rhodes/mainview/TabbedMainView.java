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
import java.util.Vector;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.file.RhoFileApi;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.webkit.WebView;
import android.widget.FrameLayout;
import android.widget.TabHost;
import android.widget.TabWidget;

public class TabbedMainView implements MainView {
	
	private static final String TAG = "TabbedMainView";
	
	private TabHost host;
	private Vector<TabData> tabData;
	private int tabIndex;
	
	private static class TabData {
		public MainView view;
		public String url;
		public boolean reload;
		public boolean loaded;
		
		public TabData() {
			loaded = false;
		}
	};
	
	private static class TabViewFactory implements TabHost.TabContentFactory {
		
		private TabData data;
		
		public TabViewFactory(TabData d) {
			data = d;
		}
		
		public View createTabContent(String tag) {
			return data.view.getView();
		}
		
	};
	
	private MainView getView(int index) {
		if (index == -1)
			index = activeTab();
		TabData data = tabData.elementAt(index);
		return data.view;
	}
	
	@SuppressWarnings("unchecked")
	public TabbedMainView(Object params) {
		RhodesService r = RhodesService.getInstance();
		Context ctx = r.getContext();
		
		Vector<Object> tabs = null;
		if (params instanceof Vector<?>)
			tabs = (Vector<Object>)params;
		else if (params instanceof Map<?,?>) {
			Map<Object,Object> settings = (Map<Object,Object>)params;
			
			Object tabsObj = settings.get("tabs");
			if (tabsObj != null && (tabsObj instanceof Vector<?>))
				tabs = (Vector<Object>)tabsObj;
		}
		
		if (tabs == null)
			throw new IllegalArgumentException("No tabs specified");
		
		int size = tabs.size();
		
		host = new TabHost(ctx);
		host.setId(RhodesService.RHO_MAIN_VIEW);
		
		tabData = new Vector<TabData>(size);
		tabIndex = 0;
		
		TabWidget tabWidget = new TabWidget(ctx);
		tabWidget.setId(android.R.id.tabs);
		TabHost.LayoutParams lpt = new TabHost.LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.WRAP_CONTENT, Gravity.TOP);
		host.addView(tabWidget, lpt);
		
		FrameLayout frame = new FrameLayout(ctx);
		frame.setId(android.R.id.tabcontent);
		FrameLayout.LayoutParams lpf = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT, Gravity.BOTTOM);
		// TODO: detect tab widget height and use it here instead of hardcoded value
		lpf.setMargins(0, 64, 0, 0);
		host.addView(frame, lpf);
		
		host.setup();
		
		host.setOnTabChangedListener(new TabHost.OnTabChangeListener() {
			
			public void onTabChanged(String tabId) {
				try {
					tabIndex = Integer.parseInt(tabId);
					TabData data = tabData.elementAt(tabIndex);
					if (data.reload || !data.loaded) {
						getView(tabIndex).navigate(data.url, tabIndex);
						data.loaded = true;
					}
				}
				catch (NumberFormatException e) {
					Logger.E(TAG, e);
				}
			}
		});
		
		TabHost.TabSpec spec;
		
		for (int i = 0; i < size; ++i) {
			Object param = tabs.elementAt(i);
			if (!(param instanceof Map<?,?>))
				throw new IllegalArgumentException("Hash expected");
			
			Map<Object, Object> hash = (Map<Object, Object>)param;
			
			Object labelObj = hash.get("label");
			if (labelObj == null || !(labelObj instanceof String))
				throw new IllegalArgumentException("'label' should be String");
			
			Object actionObj = hash.get("action");
			if (actionObj == null || !(actionObj instanceof String))
				throw new IllegalArgumentException("'action' should be String");
			
			String label = (String)labelObj;
			String action = r.normalizeUrl((String)actionObj);
			String icon = null;
			boolean reload = false;
			
			Object iconObj = hash.get("icon");
			if (iconObj != null && (iconObj instanceof String))
				icon = "apps/" + (String)iconObj;
			
			Object reloadObj = hash.get("reload");
			if (reloadObj != null && (reloadObj instanceof String))
				reload = ((String)reloadObj).equalsIgnoreCase("true");
			
			spec = host.newTabSpec(Integer.toString(i));
			
			// Set label and icon
			BitmapDrawable drawable = null;
			if (icon != null) {
				String iconPath = RhoFileApi.normalizePath(icon);
				Bitmap bitmap = BitmapFactory.decodeStream(RhoFileApi.open(iconPath));
				if (bitmap != null)
					drawable = new BitmapDrawable(bitmap);
			}
			if (drawable == null)
				spec.setIndicator(label);
			else
				spec.setIndicator(label, drawable);
			
			// Set view factory
			SimpleMainView view = new SimpleMainView();
			TabData data = new TabData();
			data.view = view;
			data.url = action;
			data.reload = reload;
			
			TabViewFactory factory = new TabViewFactory(data);
			spec.setContent(factory);
			
			tabData.addElement(data);
			host.addTab(spec);
		}
	}

	public View getView() {
		return host;
	}
	
	public WebView detachWebView() {
		return getView(activeTab()).detachWebView();
	}
	
	public void back(int index) {
		getView(index).back(0);
	}
	
	public void forward(int index) {
		getView(index).forward(0);
	}
	
	public void navigate(String url, int index) {
		getView(index).navigate(url, 0);
	}
	
	public void reload(int index) {
		getView(index).reload(0);
	}
	
	public String currentLocation(int index) {
		return getView(index).currentLocation(0);
	}

	public void switchTab(int index) {
		host.setCurrentTab(index);
		tabIndex = index;
	}
	
	public int activeTab() {
		return tabIndex;
	}

	public void goBack() {
		getView(activeTab()).goBack();
	}

	public void loadData(String data, int index) {
		getView(index).loadData(data, 0);
	}
	
	public void addNavBar(String title, Map<Object,Object> left, Map<Object,Object> right) {
		getView(activeTab()).addNavBar(title, left, right);
	}
	
	public void removeNavBar() {
		getView(activeTab()).removeNavBar();
	}

}

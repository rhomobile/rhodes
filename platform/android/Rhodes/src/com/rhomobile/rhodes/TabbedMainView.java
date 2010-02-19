package com.rhomobile.rhodes;

import java.util.Vector;

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
	
	private TabHost host;
	private Vector<WebView> views;
	
	private static class TabViewFactory implements TabHost.TabContentFactory {
		
		private WebView view;
		private String url;
		
		public TabViewFactory(WebView v, String u) {
			view = v;
			url = u;
		}
		
		public View createTabContent(String tag) {
			view.loadUrl(url);
			return view;
		}
		
	};
	
	private WebView getWebView(int index) {
		return views.elementAt(index);
	}
	
	public TabbedMainView(Vector<String> params) {
		if (params.size() % 4 != 0)
			throw new IllegalArgumentException();
		
		Rhodes r = RhodesInstance.getInstance();
		Context ctx = r.getApplicationContext();
		
		int size = params.size()/4;
		
		host = new TabHost(ctx);
		views = new Vector<WebView>(size);
		
		TabWidget tabs = new TabWidget(ctx);
		tabs.setId(android.R.id.tabs);
		TabHost.LayoutParams lpt = new TabHost.LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT, Gravity.TOP);
		host.addView(tabs, lpt);
		
		FrameLayout frame = new FrameLayout(ctx);
		frame.setId(android.R.id.tabcontent);
		FrameLayout.LayoutParams lpf = new FrameLayout.LayoutParams(LayoutParams.FILL_PARENT,
				LayoutParams.FILL_PARENT, Gravity.BOTTOM);
		// TODO: detect tab widget height and use it here instead of hardcoded value
		lpf.setMargins(0, 63, 0, 0);
		host.addView(frame, lpf);
		
		host.setup();
		
		TabHost.TabSpec spec;
		
		for (int i = 0; i < size; ++i) {
			int index = i*4;
			String label = params.elementAt(index++);
			String location = r.normalizeUrl(params.elementAt(index++));
			String icon = r.getRootPath() + "/apps/" + params.elementAt(index++);
			//boolean reload = params.elementAt(index++).equalsIgnoreCase("true");
			
			spec = host.newTabSpec(Integer.toString(i));
			
			// Set label and icon
			Bitmap bitmap = BitmapFactory.decodeFile(icon);
			BitmapDrawable drawable = null;
			if (bitmap != null)
				drawable = new BitmapDrawable(bitmap);
			if (drawable == null)
				spec.setIndicator(label);
			else
				spec.setIndicator(label, drawable);
			
			// Set view factory
			WebView view = r.createWebView();
			TabViewFactory factory = new TabViewFactory(view, location);
			spec.setContent(factory);
			
			views.addElement(view);
			host.addTab(spec);
		}
	}

	public View getView() {
		return host;
	}
	
	public void back(int index) {
		getWebView(index).goBack();
	}
	
	public void forward(int index) {
		getWebView(index).goForward();
	}
	
	public void navigate(String url, int index) {
		getWebView(index).loadUrl(url);
	}
	
	public void reload(int index) {
		getWebView(index).reload();
	}
	
	public String currentLocation(int index) {
		return getWebView(index).getUrl();
	}

	public void switchTab(int index) {
		host.setCurrentTab(index);
	}
	
	public int activeTab() {
		return host.getCurrentTab();
	}

}

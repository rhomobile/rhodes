package com.rhomobile.rhodes;

import java.util.HashMap;
import java.util.Map;

import android.view.Menu;
import android.view.MenuItem;

public class RhoMenu {
	
	private static final String TAG = "Menu";
	
	private static class Item
	{
		public String type;
		public String url;
		
		public Item(String t, String u) {
			type = t;
			url = u;
		}
	};
	
	private Map<MenuItem, Item> items;

	private native long allocMenu();
	private native void deallocMenu(long menu);
	
	private native int getMenuSize(long menu);
	private native String getMenuItemLabel(long menu, int index);
	private native String getMenuItemType(long menu, int index);
	private native String getMenuItemUrl(long menu, int index);
	
	public RhoMenu(Menu menu) {
		menu.clear();
		items = new HashMap<MenuItem, Item>();
		
		long m = allocMenu();
		for (int i = 0, lim = getMenuSize(m); i < lim; ++i) {
			String type = getMenuItemType(m, i);
			String url = getMenuItemUrl(m, i);
			String label = getMenuItemLabel(m, i);
			if (type.equalsIgnoreCase("unknown"))
				continue;
			
			MenuItem item = menu.add(label);
			items.put(item, new Item(type, url));
		}
		deallocMenu(m);
	}
	
	public boolean onMenuItemSelected(MenuItem item) {
		Object obj = items.get(item);
		if (obj == null || !(obj instanceof Item)) {
			Logger.E(TAG, "Unknown MenuItem");
			return false;
		}
		
		Item ri = (Item)obj;
		
		String type = ri.type;
		if (type.equalsIgnoreCase("log"))
			RhodesService.showLogView();
		else if (type.equalsIgnoreCase("logOptions"))
			RhodesService.showLogOptions();
		else {
			String url = ri.type;
			if (url.equalsIgnoreCase("url"))
				url = ri.url;
			RhodesService.loadUrl(url);
		}
		
		return true;
	}
	
}

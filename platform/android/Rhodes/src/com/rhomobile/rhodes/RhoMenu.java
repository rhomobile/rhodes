package com.rhomobile.rhodes;

import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.mainview.MainView;

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
		
		RhodesService r = RhodesService.getInstance();
		MainView mainView = r.getMainView();
		
		RhodesAppOptions opts = r.getOptions();
		
		String type = ri.type;
		if (type.equalsIgnoreCase("refresh")) {
			mainView.reload(mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("home")) {
			mainView.navigate(opts.getStartUrl(), mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("back")) {
			mainView.back(mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("forward")) {
			mainView.forward(mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("options")) {
			mainView.navigate(opts.getOptionsUrl(), mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("sync")) {
			r.doSyncAllSources(true);
		}
		else if (type.equalsIgnoreCase("log")) {
			RhoService.showLogView();
		}
		else if (type.equalsIgnoreCase("logOptions")) {
			RhoService.showLogOptions();
		}
		else if (type.equalsIgnoreCase("close") || type.equalsIgnoreCase("exit")) {
			RhoService.exit();
		}
		else if (type.equalsIgnoreCase("url")) {
			String url = ri.url;
			mainView.navigate(r.normalizeUrl(url), mainView.activeTab());
		}
		else {
			Logger.E(TAG, "Unknown type of menu item: \"" + type + "\"");
			return false;
		}
		return true;
	}
	
}

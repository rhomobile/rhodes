package com.rhomobile.rhodes;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import com.rhomobile.rhodes.mainview.MainView;

import android.view.Menu;
import android.view.MenuItem;

public class RhoMenu {
	
	private static final String TAG = "Menu";

	private Map<MenuItem, Integer> hash;
	private Vector<String> types;
	private Vector<String> urls;
	
	private native int getMenuSize();
	private native String getMenuItemLabel(int index);
	private native String getMenuItemType(int index);
	private native String getMenuItemUrl(int index);
	
	public RhoMenu(Menu menu) {
		hash = new HashMap<MenuItem, Integer>();
		urls = new Vector<String>();
		types = new Vector<String>();
		for (int i = 0, lim = getMenuSize(); i < lim; ++i) {
			String type = getMenuItemType(i);
			String label = getMenuItemLabel(i);
			String url = getMenuItemUrl(i);
			if (!type.equalsIgnoreCase("unknown")) {
				MenuItem item = menu.add(label);
				hash.put(item, new Integer(i));
			}
			types.addElement(type);
			urls.addElement(url);
		}
	}
	
	public boolean onMenuItemSelected(MenuItem item) {
		Object obj = hash.get(item);
		if (obj == null || !(obj instanceof Integer))
			return false;
		
		int index = ((Integer)obj).intValue();
		
		Rhodes r = RhodesInstance.getInstance();
		MainView mainView = r.getMainView();
		
		String type = types.elementAt(index);
		if (type.equalsIgnoreCase("refresh")) {
			mainView.reload(mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("home")) {
			mainView.navigate(r.getStartUrl(), mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("back")) {
			mainView.back(mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("forward")) {
			mainView.forward(mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("options")) {
			mainView.navigate(r.getOptionsUrl(), mainView.activeTab());
		}
		else if (type.equalsIgnoreCase("sync")) {
			r.doSyncAllSources(true);
		}
		else if (type.equalsIgnoreCase("log")) {
			Rhodes.showLogView();
		}
		else if (type.equalsIgnoreCase("logOptions")) {
			Rhodes.showLogOptions();
		}
		else if (type.equalsIgnoreCase("close") || type.equalsIgnoreCase("exit")) {
			r.stopSelf();
		}
		else if (type.equalsIgnoreCase("url")) {
			String url = urls.elementAt(index);
			mainView.navigate(r.normalizeUrl(url), mainView.activeTab());
		}
		else {
			Logger.E(TAG, "Unknown type of menu item: \"" + type + "\"");
			return false;
		}
		return true;
	}
	
}

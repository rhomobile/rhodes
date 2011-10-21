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

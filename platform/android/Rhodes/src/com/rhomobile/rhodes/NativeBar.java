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

import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SimpleMainView;
import com.rhomobile.rhodes.mainview.TabbedMainView;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class NativeBar {
	
	private static final String TAG = "NativeBar";
	
	public static final int TOOLBAR_TYPE = 0;
	public static final int TABBAR_TYPE = 1;
	public static final int NOBAR_TYPE = 2;
	
	private static boolean started = false;
	
	private static class CreateTask implements Runnable {
		
		private int type;
		private Object params;
		
		public CreateTask(int t, Object p) {
			type = t;
			params = p;
		}

		public void run() {
			try {
				RhodesService r = RhodesService.getInstance();
				MainView mainView = r.getMainView();
				MainView v = null;
				
				switch (type) {
				case NOBAR_TYPE:
					v = new SimpleMainView(mainView);
					started = false;
					break;
				case TOOLBAR_TYPE:
					v = new SimpleMainView(mainView, params);
					started = true;
					break;
				case TABBAR_TYPE:
					v = new TabbedMainView(params);
					started = true;
					break;
				default:
					Logger.E(TAG, "Unknown bar type passed: " + type);
				}
				
				if (v != null)
					r.setMainView(v);
			}
			catch (Exception e) {
				reportFail("CreateTask", e);
			}
		}
		
	};
	
	private static class SwitchTabTask implements Runnable {
		
		private int index;
		
		public SwitchTabTask(int i) {
			index = i;
		}
		
		public void run() {
			RhodesService.getInstance().getMainView().switchTab(index);
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}

	public static void create(int type, Object params) {
		try {
			PerformOnUiThread.exec(new CreateTask(type, params), false);
		}
		catch (Exception e) {
			reportFail("create", e);
		}
	}
	
	public static void remove() {
		try {
			PerformOnUiThread.exec(new CreateTask(NOBAR_TYPE, null), false);
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}
	
	public static void switchTab(int index) {
		try {
			PerformOnUiThread.exec(new SwitchTabTask(index), false);
		}
		catch (Exception e) {
			reportFail("switchTab", e);
		}
	}
	
	public static boolean isStarted() {
		return started;
	}
}

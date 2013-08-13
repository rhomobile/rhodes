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

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
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
	private static boolean toolbar_started = false;
	private static boolean tabbar_started = false;
	
	private static class CreateTask implements Runnable {
		
		private int type;
		private Object params;
		private Object options;
		private IMethodResult callback;
		
		public CreateTask(int t, Object p, Object o, IMethodResult cb) {
			type = t;
			params = p;
			options = o;
			callback = cb;
		}

		public void run() {
			try {
				MainView mainView = RhodesActivity.safeGetInstance().getMainView();
				MainView v = null;
				
				SimpleMainView smv = null;
				if (mainView instanceof SimpleMainView) {
					smv = (SimpleMainView)mainView;
				}
				
				switch (type) {
				case NOBAR_TYPE:
                    if (smv == null) {
                        IRhoWebView webView = mainView.detachWebView();
                        v = new SimpleMainView(webView);
                    } else
						smv.removeToolbar();
					started = false;
					toolbar_started = false;
					tabbar_started = false;
					break;
				case TOOLBAR_TYPE:
                    if (smv == null) {
                        IRhoWebView webView = mainView.detachWebView();
                        v = new SimpleMainView(webView, params);
                    }
					else
						smv.setToolbar(params, options);
					started = true;
					toolbar_started = true;
					tabbar_started = false;
					break;
				case TABBAR_TYPE:
					v = new TabbedMainView(params, options, callback);
					started = true;
					toolbar_started = false;
					tabbar_started = true;
					break;
				default:
					Logger.E(TAG, "Unknown bar type passed: " + type);
				}
				
				if (v != null) {
				    mainView.destroy();
				    RhodesActivity.safeGetInstance().setMainView(v);
					if (v instanceof TabbedMainView) {
						// loading of default opened tab should be after TabView insert to real Views tree
						TabbedMainView tmv = (TabbedMainView)v;
						tmv.loadFirstPage();
					}
				}
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

	public static void create(int type, Object params, Object options, IMethodResult callback) {
		try {
			PerformOnUiThread.exec(new CreateTask(type, params, options, callback));
		}
		catch (Exception e) {
			reportFail("create", e);
		}
	}
	
	public static void remove() {
		try {
			PerformOnUiThread.exec(new CreateTask(NOBAR_TYPE, null, null, null));
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}

	public static void removeToolbar() {
		try {
			if (toolbar_started) {
				PerformOnUiThread.exec(new CreateTask(NOBAR_TYPE, null, null, null));
			}
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}
	
	public static void removeTabbar() {
		try {
			if (tabbar_started) {
				PerformOnUiThread.exec(new CreateTask(NOBAR_TYPE, null, null, null));
			}
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}
	
	public static void switchTab(int index) {
		try {
			PerformOnUiThread.exec(new SwitchTabTask(index));
		}
		catch (Exception e) {
			reportFail("switchTab", e);
		}
	}
	
	public static boolean isStarted() {
		return started;
	}
	
	public static boolean isToolbarStarted() {
		return toolbar_started;
	}

	public static boolean isTabbarStarted() {
		return tabbar_started;
	}
	
	public static int activeTab() {
		RhodesService r = RhodesService.getInstance();
		MainView mainView = r.getMainView();
		return mainView.activeTab();
	}
}

package com.rhomobile.rhodes;

import java.util.Vector;

public class NativeBar {
	
	private static final String TAG = "NativeBar";
	
	public static final int TOOLBAR_TYPE = 0;
	public static final int TABBAR_TYPE = 1;
	public static final int NOBAR_TYPE = 2;
	
	private static class CreateTask implements Runnable {
		
		private int type;
		private Vector<String> params;
		
		public CreateTask(int t, Vector<String> p) {
			type = t;
			params = p;
		}

		public void run() {
			Rhodes r = RhodesInstance.getInstance();
			MainView v = null;
			
			switch (type) {
			case NOBAR_TYPE:
			case TOOLBAR_TYPE:
				// There is no toolbar on android so TOOLBAR_TYPE means exactly NOBAR_TYPE
				v = new SimpleMainView();
				break;
			case TABBAR_TYPE:
				v = new TabbedMainView(params);
				break;
			default:
				Logger.E(TAG, "Unknown bar type passed: " + type);
			}
			
			if (v != null)
				r.setMainView(v);
		}
		
	};
	
	private static class SwitchTabTask implements Runnable {
		
		private int index;
		
		public SwitchTabTask(int i) {
			index = i;
		}
		
		public void run() {
			RhodesInstance.getInstance().getMainView().switchTab(index);
		}
	};
	
	public static void create(int type, Vector<String> params) {
		Rhodes.performOnUiThread(new CreateTask(type, params));
	}
	
	public static void remove() {
		Rhodes.performOnUiThread(new CreateTask(NOBAR_TYPE, null));
	}
	
	public static void switchTab(int index) {
		Rhodes.performOnUiThread(new SwitchTabTask(index));
	}
	
}

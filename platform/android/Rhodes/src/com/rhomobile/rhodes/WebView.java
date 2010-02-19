package com.rhomobile.rhodes;

public class WebView {
	
	private static class NavigateTask implements Runnable {
		private String url;
		private int index;
		
		public NavigateTask(String u, int i) {
			url = u;
			index = i;
		}
		
		public void run() {
			RhodesInstance.getInstance().getMainView().navigate(url, index);
		}
	};
	
	private static class RefreshTask implements Runnable {
		private int index;
		
		public RefreshTask(int i) {
			index = i;
		}
		
		public void run() {
			RhodesInstance.getInstance().getMainView().reload(index);
		}
	};
	
	private static class LocationTask implements Runnable {
		private int index;
		private StringBuffer loc;
		
		public LocationTask(int i, StringBuffer l) {
			index = i;
			loc = l;
		}
		
		public void run() {
			loc.delete(0, -1);
			loc.append(RhodesInstance.getInstance().getMainView().currentLocation(index));
		} 
	};
	
	private static class IntHolder {
		public int value;
	};
	
	private static class ActiveTabTask implements Runnable {
		private IntHolder ret;
		
		public ActiveTabTask(IntHolder r) {
			ret = r;
		}
		
		public void run() {
			ret.value = RhodesInstance.getInstance().getMainView().activeTab();
		}
	};

	public static void navigate(String url, int index) {
		Rhodes.performOnUiThread(new NavigateTask(url, index));
	}
	
	public static void refresh(int index) {
		Rhodes.performOnUiThread(new RefreshTask(index));
	}
	
	public static String currentLocation(int index) {
		StringBuffer loc = new StringBuffer();
		Rhodes.performOnUiThread(new LocationTask(index, loc));
		return loc.toString();
	}
	
	public static int activeTab() {
		IntHolder v = new IntHolder();
		Rhodes.performOnUiThread(new ActiveTabTask(v));
		return v.value;
	}
	
	public static String executeJs(String js, int index) {
		Rhodes.performOnUiThread(new NavigateTask("javascript:" + js, index));
		return "";
	}
}

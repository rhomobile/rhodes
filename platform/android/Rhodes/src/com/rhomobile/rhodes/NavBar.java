package com.rhomobile.rhodes;

import java.util.Map;

import com.rhomobile.rhodes.util.PerformOnUiThread;

public class NavBar {

	private static final String TAG = "NavBar";
	
	private static boolean started = false;
	
	private static class CreateTask implements Runnable {
		
		private String title;
		private Map<Object, Object> left;
		private Map<Object, Object> right;
		
		@SuppressWarnings("unchecked")
		public CreateTask(Map<Object, Object> p) {
			Object titleObj = p.get("title");
			if (titleObj == null || !(titleObj instanceof String))
				throw new IllegalArgumentException("'title' should be String");
			title = (String)titleObj;
			
			Object leftObj = p.get("left");
			if (leftObj == null || !(leftObj instanceof Map<?,?>))
				throw new IllegalArgumentException("'left' - expected Hash");
			left = (Map<Object,Object>)leftObj;
			
			Object rightObj = p.get("right");
			if (rightObj != null && !(rightObj instanceof Map<?,?>))
				throw new IllegalArgumentException("'right' - expected Hash");
			right = (Map<Object,Object>)rightObj;
		}
		
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().addNavBar(title, left, right);
			started = true;
		}
	};
	
	private static class RemoveTask implements Runnable {
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().removeNavBar();
			started = false;
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static void create(Map<Object, Object> params) {
		try {
			PerformOnUiThread.exec(new CreateTask(params), false);
		}
		catch (Exception e) {
			reportFail("create", e);
		}
	}
	
	public static void remove() {
		try {
			PerformOnUiThread.exec(new RemoveTask(), false);
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}
	
	public static boolean isStarted() {
		return started;
	}
}

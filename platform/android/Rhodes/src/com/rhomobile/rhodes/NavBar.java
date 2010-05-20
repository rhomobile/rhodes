package com.rhomobile.rhodes;

import java.util.Map;
import java.util.Vector;

public class NavBar {

	private static final String TAG = "NavBar";
	
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
			Rhodes r = RhodesInstance.getInstance();
			r.getMainView().setNavBar(title, left, right);
		}
	};
	
	private static class RemoveTask implements Runnable {
		public void run() {
			Rhodes r = RhodesInstance.getInstance();
			r.getMainView().removeNavBar();
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static void create(Map<Object, Object> params) {
		try {
			Rhodes.performOnUiThread(new CreateTask(params), false);
		}
		catch (Exception e) {
			reportFail("create", e);
		}
	}
	
	public static void remove() {
		try {
			Rhodes.performOnUiThread(new RemoveTask(), false);
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}
	
}

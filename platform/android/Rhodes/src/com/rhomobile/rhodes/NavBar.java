package com.rhomobile.rhodes;

import java.util.Vector;

public class NavBar {

	private static final String TAG = "NavBar";
	
	private static class CreateTask implements Runnable {
		
		private String title;
		private Vector<Object> left;
		private Vector<Object> right;
		
		public CreateTask(Vector<Object> p) {
			// TODO: implement
		}
		
		public void run() {
			// TODO: implement
		}
	};
	
	private static class RemoveTask implements Runnable {
		public void run() {
			// TODO: implement
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static void create(Vector<Object> params) {
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

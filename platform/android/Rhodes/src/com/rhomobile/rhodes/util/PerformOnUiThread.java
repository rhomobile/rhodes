package com.rhomobile.rhodes.util;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;

public class PerformOnUiThread implements Runnable {
	
	private static final String TAG = PerformOnUiThread.class.getSimpleName();
	
	private Runnable runnable;
	
	public PerformOnUiThread(Runnable r) {
		runnable = r;
	}
	
	public void run() {
		try {
			runnable.run();
		}
		catch (Exception e) {
			Logger.E(TAG, "Operation failed: " + e.getMessage());
		}
		finally {
			synchronized (runnable) {
				runnable.notify();
			}
		}
	}
	
	public static void exec(Runnable r, int delay) {
		try {
			RhodesActivity ra = RhodesActivity.getInstance();
			ra.post(r, delay);
		}
		catch (Exception e) {
			Logger.E(TAG, "exec failed: " + e.getMessage());
		}
	}
	
	public static void exec(Runnable r, boolean wait) {
		try {
			RhodesActivity ra = RhodesActivity.getInstance();
			if (!wait) {
				ra.post(r);
			}
			else {
				long thrId = Thread.currentThread().getId();
				if (ra.getUiThreadId() == thrId) {
					// We are already in UI thread
					r.run();
				}
				else {
					// Post request to UI thread and wait when it would be done
					synchronized (r) {
						ra.post(new PerformOnUiThread(r));
						r.wait();
					}
				}
			}
		}
		catch (Exception e) {
			Logger.E(TAG, "exec failed: " + e.getMessage());
		}
	}
};

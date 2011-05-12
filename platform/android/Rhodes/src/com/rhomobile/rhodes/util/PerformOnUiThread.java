package com.rhomobile.rhodes.util;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;

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
	
	public static void exec(final Runnable r, final int delay) {
        RhodesApplication.runWhen(
                RhodesApplication.UiState.MainActivityCreated,
                new RhodesApplication.StateHandler() {
                    @Override
                    public boolean run() {
                        try {
                            RhodesActivity.safeGetInstance().post(r, delay);
                        }
                        catch (Exception e) {
                            Logger.E(TAG, "exec failed: " + e.getMessage());
                            setError(e);
                        }
                        return true;
                    }
                });
    }

    @Deprecated
	public static void exec(final Runnable r, final boolean wait) {
        if (wait)
            sync_exec(r);
        else
            exec(r);
    }

    public static void exec(final Runnable r) {
        RhodesApplication.runWhen(
                RhodesApplication.UiState.MainActivityCreated,
                new RhodesApplication.StateHandler() {
                    @Override
                    public boolean run() {
                        try {
                            RhodesActivity.safeGetInstance().post(r);
                        } catch (Exception e) {
                            Logger.E(TAG, "exec failed: " + e.getMessage());
                            setError(e);
                        }
                        return true;
                    }
                });
    }
    
    
    // Special exec edition for RhoBluetoothManager
    //TODO: Use future pattern to return result and wait
    @Deprecated
    public static void sync_exec(final Runnable r)
    {
        try {
            RhodesActivity ra = RhodesActivity.safeGetInstance();
            
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
        } catch (Exception e) {
            Logger.E(TAG, "exec failed: " + e.getMessage());
            Thread.dumpStack();
        }
    }
};

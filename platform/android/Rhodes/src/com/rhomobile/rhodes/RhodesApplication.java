package com.rhomobile.rhodes;

import java.util.ArrayList;
import java.util.List;

import android.app.Application;
import android.os.Process;

public class RhodesApplication extends Application {
	
	private List<Runnable> mOnExit = new ArrayList<Runnable>();
	
	static {
		NativeLibraries.load();
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
	}
	
	public void addOnExitHandler(Runnable action) {
		synchronized (this) {
			mOnExit.add(action);
		}
	}
	
	public boolean removeOnExitHandler(Runnable action) {
		synchronized (this) {
			return mOnExit.remove(action);
		}
	}
	
	public void exit() {
		List<Runnable> onExit = new ArrayList<Runnable>();
		synchronized (this) {
			onExit.addAll(mOnExit);
		}
		
		for (Runnable action : onExit) {
			action.run();
		}
		
		try {
			RhodesService r = RhodesService.getInstance();
			if (r != null)
				r.stopSelf();
		}
		catch (Exception e) {}
		
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra != null)
			ra.finish();
		
		Process.killProcess(Process.myPid());
	}
	
}

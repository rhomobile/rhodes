package com.rhomobile.rhodes;

import android.app.Activity;
import android.app.Application;
import android.util.Log;

public class BaseActivity extends Activity {
	
	private static final String TAG = BaseActivity.class.getName();
	
	private static final boolean DEBUG = false;

	@Override
	protected void onStart() {
		super.onStart();
		/*
		if (RhodesService.getInstance() != null) {
			RhodesService.getInstance().activityStarted();
		}
		*/
	}
	
	@Override
	protected void onStop() {
		//RhodesService.getInstance().activityStopped();
		super.onStop();
	}
	
	public RhodesApplication getRhodesApplication() {
		Application app = super.getApplication();
		if (DEBUG)
			Log.d(TAG, "getApplication: " + app);
		return (RhodesApplication)app;
	}
}

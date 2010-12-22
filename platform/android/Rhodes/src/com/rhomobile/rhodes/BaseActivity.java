package com.rhomobile.rhodes;

import android.app.Activity;
import android.app.Application;
import android.content.res.Configuration;
import android.util.Log;

public class BaseActivity extends Activity {
	
	private static final String TAG = BaseActivity.class.getName();
	
	private static final boolean DEBUG = false;

	@Override
	protected void onStart() {
		super.onStart();
		RhodesService.activityStarted();
	}
	
	@Override
	protected void onStop() {
		RhodesService.activityStopped();
		super.onStop();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Logger.T(TAG, "+++ onConfigurationChanged");
		super.onConfigurationChanged(newConfig);
		RhodesService.getInstance().rereadScreenProperties();
	}
	
	public RhodesApplication getRhodesApplication() {
		Application app = super.getApplication();
		if (DEBUG)
			Log.d(TAG, "getApplication: " + app);
		return (RhodesApplication)app;
	}
}

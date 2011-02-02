package com.rhomobile.rhodes;

import android.app.Activity;
import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

public class BaseActivity extends Activity implements ServiceConnection {
	
	private static final String TAG = BaseActivity.class.getName();
	
	private static final boolean DEBUG = false;
	
	protected RhodesService mRhodesService;
	private boolean mBoundToService;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Intent intent = new Intent(this, RhodesService.class);
		ComponentName serviceName = startService(intent);
		if (serviceName == null)
			throw new RuntimeException("Can not start Rhodes service");
		bindService(intent, this, Context.BIND_AUTO_CREATE);
		mBoundToService = true;
	}
	
	@Override
	protected void onDestroy() {
		if (mBoundToService) {
			unbindService(this);
			mBoundToService = false;
		}
		super.onDestroy();
	}
	
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

	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		mRhodesService = ((RhodesService.LocalBinder)service).getService();
		if (DEBUG)
			Log.d(TAG, "Connected to service: " + mRhodesService);
	}

	@Override
	public void onServiceDisconnected(ComponentName name) {
		if (DEBUG)
			Log.d(TAG, "Disconnected from service: " + mRhodesService);
		mRhodesService = null;
	}
}

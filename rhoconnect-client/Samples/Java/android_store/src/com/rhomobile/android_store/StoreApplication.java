package com.rhomobile.android_store;

import android.app.Application;

public class StoreApplication extends Application {

	private static final String TAG = StoreApplication.class.getSimpleName();
	
	@Override
	public void onCreate()
	{
		System.loadLibrary("rhoconnectclient");
	}
	
}

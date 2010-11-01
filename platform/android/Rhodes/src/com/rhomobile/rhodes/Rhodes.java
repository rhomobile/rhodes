/*
 ============================================================================
 Author	    : Anton Antonov
 Version	: 1.0
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes;

import java.util.Timer;
import java.util.TimerTask;

import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.view.Window;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;
import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;

public class Rhodes extends RhoActivity {

	private static final String TAG = "Rhodes";
	
	private static Rhodes instance = null;
	
	public static Rhodes getInstance() {
		return instance;
	}
	
	private RhoMenu appMenu = null;
	
	private ViewGroup mOuterFrame = null;
	private Bundle mSavedBundle = null;
	private SplashScreen mSplashScreen = null;
	private Handler mHandler = null;
		
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		RhodesService.platformLog("Rhodes", "onStart()");
		
		super.onCreate(savedInstanceState);
		mHandler = new Handler();

		// Here Log should be used, not Logger. It is because Logger is not initialized yet.
		Log.v(TAG, "+++ onCreate");
		
		instance = this;
		
		FrameLayout v = new FrameLayout(this);
		mOuterFrame = v;
		
		mSplashScreen = RhodesService.showSplashScreen(this, mOuterFrame);
		mSavedBundle = savedInstanceState;

		getWindow().setFlags(RhodesService.WINDOW_FLAGS, RhodesService.WINDOW_MASK);

		
		this.requestWindowFeature(Window.FEATURE_PROGRESS);

		getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 10000);
		
		setContentView(mOuterFrame, new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));


	}
	
	public void onStart() {
		super.onStart();
		RhodesService.platformLog("Rhodes", "onStart()");
	}
	
	public void onCreatePosponed() {
		RhodesService.platformLog("Rhodes", "onCreatePosponed()");
		
		RhodesService service = RhodesService.getInstance();
		if (service == null) {
			Log.v(TAG, "Starting rhodes service...");
			service = new RhodesService(this, mOuterFrame, mSplashScreen);
		}
		else
			Log.v(TAG, "Rhodes service already started...");
		
		Thread ct = Thread.currentThread();
		ct.setPriority(Thread.MAX_PRIORITY);
		service.setInfo(this, ct.getId(), mHandler);


		boolean disableScreenRotation = RhoConf.getBool("disable_screen_rotation");
		this.setRequestedOrientation(disableScreenRotation ? ActivityInfo.SCREEN_ORIENTATION_PORTRAIT :
			ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
		
		//if (RhodesService.ENABLE_LOADING_INDICATION)
		//	this.requestWindowFeature(Window.FEATURE_PROGRESS);

		service.setRootWindow(mOuterFrame);
	
		RhodesService.getInstance().activityStarted();
		
		if (mTimerPostponeCreate != null) {
			mTimerPostponeCreate.cancel();
			mTimerPostponeCreate = null;
		}
	}
	
	private Timer mTimerPostponeCreate = null;
	
	public void onResume() {
		RhodesService.platformLog("Rhodes", "onResume()");
		
		super.onResume();

	}
	
	public static void runPosponedSetup() {
		final Rhodes r = Rhodes.getInstance();
		r.mHandler.post( new Runnable() {
				public void run() {
					RhodesService.platformLog("Rhodes", "postponed Create UIThread.run()");
					r.onCreatePosponed();
				}
			});
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Logger.T(TAG, "+++ onConfigurationChanged");
		super.onConfigurationChanged(newConfig);
		RhodesService.getInstance().rereadScreenProperties();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			RhodesService r = RhodesService.getInstance();
			MainView v = r.getMainView();
			v.back(v.activeTab());
			return true;
		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		super.onPrepareOptionsMenu(menu);
		appMenu = new RhoMenu(menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		if (appMenu == null)
			return false;
		return appMenu.onMenuItemSelected(item);
	}
	
	static {
		NativeLibraries.load();
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		RhoBluetoothManager.onActivityResult(requestCode, resultCode, data);
	}
	
	
}

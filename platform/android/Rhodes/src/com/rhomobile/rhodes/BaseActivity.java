/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.IBinder;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

public class BaseActivity extends Activity implements ServiceConnection {
	
	private static final String TAG = BaseActivity.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	public static final String INTENT_SOURCE = BaseActivity.class.getName();
	
    public static class ScreenProperties {
        private int mScreenWidth;
        private int mScreenHeight;
        private int mScreenOrientation;
        private float mScreenPpiX;
        private float mScreenPpiY;
        
        ScreenProperties(Context context) {
            reread(context);
        }
        
        public void reread(Context context) {
            Logger.T(TAG, "Updating screen properties");
            
            WindowManager wm = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);

            Display d = wm.getDefaultDisplay();

            mScreenWidth = d.getWidth();
            mScreenHeight = d.getHeight();

            DisplayMetrics metrics = new DisplayMetrics();
            d.getMetrics(metrics);

            mScreenPpiX = metrics.xdpi;
            mScreenPpiY = metrics.ydpi;
            
            int orientation;
            orientation = context.getResources().getConfiguration().orientation;
            Logger.D(TAG, "Resources orientation: "+ orientation);

            if (orientation == Configuration.ORIENTATION_UNDEFINED)
            {
                orientation = d.getOrientation();
                Logger.D(TAG, "DisplayMetrics orientation: "+ orientation);

                if (orientation == Configuration.ORIENTATION_UNDEFINED) {
                    if (d.getWidth() == d.getHeight())
                        orientation = Configuration.ORIENTATION_SQUARE;
                    else if(d.getWidth() < d.getHeight())
                        orientation = Configuration.ORIENTATION_PORTRAIT;
                    else
                        orientation = Configuration.ORIENTATION_LANDSCAPE;

                    Logger.D(TAG, "Screen resolution orientation: " + orientation);
                }
            }
            mScreenOrientation = orientation;

            Logger.D(TAG, "New screen properties - width: " + mScreenWidth + ", height: " + mScreenHeight + ", orientation: " + mScreenOrientation);
        }
        
        public int getWidth() { return mScreenWidth; }
        public int getHeight() { return mScreenHeight; }
        public int getOrientation() { return mScreenOrientation; }
        public float getPpiX() { return mScreenPpiX; }
        public float getPpiY() { return mScreenPpiY; }
    }

    private static ScreenProperties sScreenProp = null;
    
    public static ScreenProperties getScreenProperties() { return sScreenProp; }
    
    protected RhodesService mRhodesService;
	private boolean mBoundToService;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Logger.T(TAG, "onCreate");

		Intent intent = new Intent(this, RhodesService.class);
		intent.putExtra(RhodesService.INTENT_SOURCE, INTENT_SOURCE);
		ComponentName serviceName = startService(intent);
		if (serviceName == null)
			throw new RuntimeException("Can not start Rhodes service");
		bindService(intent, this, Context.BIND_AUTO_CREATE);
		mBoundToService = true;

        if (sScreenProp == null) {
            sScreenProp = new ScreenProperties(this);
        } else {
            if (RhoConf.getBool("disable_screen_rotation")) { 
                Logger.D(TAG, "Screen rotation is disabled. Force orientation: " + getScreenProperties().getOrientation());
                setRequestedOrientation(getScreenProperties().getOrientation());
            }
        }
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
		Logger.T(TAG, "onConfigurationChanged");
		if (RhoConf.getBool("disable_screen_rotation"))
		{
			super.onConfigurationChanged(newConfig);
            Logger.D(TAG, "Screen rotation is disabled. Force old orientation: " + getScreenProperties().getOrientation());
			setRequestedOrientation(getScreenProperties().getOrientation());
		}
		else
		{
			super.onConfigurationChanged(newConfig);
			getScreenProperties().reread(this);
		}
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

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

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;

import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;
import com.rhomobile.rhodes.camera.Camera;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SimpleMainView;
import com.rhomobile.rhodes.mainview.SplashScreen;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebView;

public class RhodesActivity extends BaseActivity {
	
	private static final String TAG = RhodesActivity.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	public static boolean ENABLE_LOADING_INDICATION = false;
	
	public static int MAX_PROGRESS = 10000;
	
	private static RhodesActivity sInstance = null;
	
	private Handler mHandler;
	
	private SplashScreen mSplashScreen;
	
	private MainView mMainView;
	
	private RhoMenu mAppMenu;

	private long uiThreadId = 0;
	
	public long getUiThreadId() {
		return uiThreadId;
	}
	
	private ArrayList<RhodesActivityListener> mListeners = null;
	
	private boolean mIsForeground = false;
	private boolean mIsInsideStartStop = false;
	
	public boolean isForegroundNow() {
		return mIsForeground;
	}
	
	public boolean isInsideStartStop() {
		return mIsInsideStartStop;
	}
	
	public void addRhodesActivityListener(RhodesActivityListener listener) {
		if (!mListeners.contains(listener)) {
			mListeners.add(listener);
		}
	}
	
	public void removeRhodesActivityListener(RhodesActivityListener listener) {
		mListeners.remove(listener);
	}
	
	public void processStartupListeners() {
		int i;
		for (i = 1; i < RhodesActivityStartupListeners.ourRunnableList.length; i++) {
			String classname = RhodesActivityStartupListeners.ourRunnableList[i];
			Class<? extends RhodesActivityListener> klass = null;
			try {
				klass = Class.forName(classname).asSubclass(RhodesActivityListener.class);
			} catch (ClassNotFoundException e) {
				Utils.platformLog("RhodesActivity", "processStartupListeners() : ClassNotFoundException for ["+classname+"]");
				e.printStackTrace();
			}
			RhodesActivityListener listener = null;
			try {
				if (klass != null) {
					listener = klass.newInstance();
				}
			} catch (InstantiationException e) {
				Utils.platformLog("RhodesActivity", "processStartupListeners() : InstantiationException for ["+classname+"]");
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				Utils.platformLog("RhodesActivity", "processStartupListeners() : IllegalAccessException for ["+classname+"]");
				e.printStackTrace();
			}
			if (listener != null) {
				listener.onRhodesActivityStartup(this);
			}
		}
	}
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        Logger.T(TAG, "onCreate");
		super.onCreate(savedInstanceState);
		
		mListeners = new ArrayList<RhodesActivityListener>();

		Thread ct = Thread.currentThread();
		//ct.setPriority(Thread.MAX_PRIORITY);
		uiThreadId = ct.getId();

		sInstance = this;
		
		Camera.init_from_UI_Thread();

		if (!RhodesService.isTitleEnabled()) {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
		}
		else {
		}
		requestWindowFeature(Window.FEATURE_PROGRESS);
        getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS);

		mHandler = new Handler();

        mSplashScreen = new SplashScreen(this);
        setMainView(mSplashScreen);

		processStartupListeners();
        {
        	Iterator<RhodesActivityListener> iterator = mListeners.iterator();
        	while (iterator.hasNext()) {
        		iterator.next().onCreate(this, getIntent());
        	}
        }
		
		notifyUiCreated();
        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityCreated);
    }

    public MainView switchToSimpleMainView(MainView currentView) {
        MainView view = new SimpleMainView(currentView.detachWebView()); 
        setMainView(view);
        return view;
    }
	
	private void notifyUiCreated() {
		RhodesService r = RhodesService.getInstance();
		if ( r != null ) {
			RhodesService.callUiCreatedCallback();
		}
		else {
			mHandler.post(new Runnable() {
				public void run() {
					RhodesService r = RhodesService.getInstance();
					if (r == null) {
						// If there is no yet running RhodesService instance,
						// try to do the same after 100ms
						mHandler.postDelayed(this, 100);
						return;
					}
				
					RhodesService.callUiCreatedCallback();
				}
			});
		}
	}
	
	public static void setFullscreen(int enable) {
		//Utils.platformLog(TAG, "setFullscreen("+String.valueOf(enable)+")");
		final int en = enable;
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
				if (en != 0) {
					getInstance().getWindow().clearFlags( WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
					getInstance().getWindow().setFlags( WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
				}
				else {
					getInstance().getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
					getInstance().getWindow().setFlags( WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN, WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
				}
			}
		});
	}

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);

        Logger.D(TAG, "onNewIntent");

        handleStartParams(intent);

        {
        	Iterator<RhodesActivityListener> iterator = mListeners.iterator();
        	while (iterator.hasNext()) {
        		iterator.next().onNewIntent(this, intent);
        	}
        }
    }

	@Override
	public void onStart() {
		super.onStart();

        Logger.D(TAG, "onStart");
        mIsInsideStartStop = true;
        
        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityStarted);
	}
	
	@Override
	public void onResume() {
        Logger.D(TAG, "onResume");
    	mIsForeground = true;
		super.onResume();
        {
        	Iterator<RhodesActivityListener> iterator = mListeners.iterator();
        	while (iterator.hasNext()) {
        		iterator.next().onResume(this);
        	}
        }
	}

    @Override
    public void onPause() 
    {
    	mIsForeground = false;
        {
        	Iterator<RhodesActivityListener> iterator = mListeners.iterator();
        	while (iterator.hasNext()) {
        		iterator.next().onPause(this);
        	}
        }

        super.onPause();
        Logger.D(TAG, "onPause");

        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityPaused);
    }

    @Override
	public void onStop() 
	{
		super.onStop();
        Logger.D(TAG, "onStop");
        mIsInsideStartStop = false;
	}
	
	@Override
	public void onDestroy() {
        Logger.D(TAG, "onDestroy");
        
        //TODO: Check is it really correct in case activity killed immediately after onPause()
        sInstance = null;
		super.onDestroy();
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			RhodesService r = RhodesService.getInstance();
			if (DEBUG)
				Logger.D(TAG, "onKeyDown: r=" + r);
			if (r == null)
				return false;
			
			MainView v = r.getMainView();
			v.goBack();//back(v.activeTab());
			return true;
		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		super.onPrepareOptionsMenu(menu);
		mAppMenu = new RhoMenu(menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		if (mAppMenu == null)
			return false;
		return mAppMenu.onMenuItemSelected(item);
	}
	
	@Deprecated
	public static RhodesActivity getInstance() {
		return sInstance;
	}

    public static RhodesActivity safeGetInstance() throws NullPointerException {
        if(sInstance != null)
            return sInstance;
        else
            throw new NullPointerException("RhodesActivity.sInstance == null");
    }

	public RhodesService getService() {
		return mRhodesService;
	}
	
	public void post(Runnable r) {
		mHandler.post(r);
	}
	
	public void post(Runnable r, long delay) {
		mHandler.postDelayed(r, delay);
	}
	
	public SplashScreen getSplashScreen() {
		return mSplashScreen;
	}

    public void setMainView(MainView v) {
        if (v != null) {
            mMainView = v;
            setContentView(v.getView());
        }
    }

	public MainView getMainView() {
		return mMainView;
	}	

	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		super.onServiceConnected(name, service);

        Logger.D(TAG, "onServiceConnected: " + name.toShortString());

        handleStartParams(getIntent());

		ENABLE_LOADING_INDICATION = !RhoConf.getBool("disable_loading_indication");
	}

    private void handleStartParams(Intent intent)
    {
        StringBuilder startParams = new StringBuilder();
        boolean firstParam = true;
        if (intent.getData() != null) {
	    	String strUri = intent.toUri(0);
	        
	        if(strUri.length() > 0)
	        {
	            Uri uri = Uri.parse(strUri);
	            String authority = uri.getAuthority();
	            String path = uri.getPath();
	            String query = uri.getQuery();
	
	            if (authority != null)
	                startParams.append(authority);
	            if (path != null)
	                startParams.append(path);
	            if (query != null) {
	                startParams.append('?').append(query);
	                firstParam = false;
	            }
	        }
        }
        Bundle extras = intent.getExtras();
        if (extras != null) {
	        Set<String> keys = extras.keySet();
	
	        for (String key : keys) {
	            Object value = extras.get(key);
	            if (firstParam) {
	            	startParams.append("?");
	            	firstParam = false;
	            } else
	            	startParams.append("&");
	            startParams.append(key);
	            if (value != null) {
	            	startParams.append("=");
	            	startParams.append(value.toString());
	            }
	        }
        }
        String paramString = startParams.toString();
        Logger.I(TAG, "New start parameters: " + paramString);
        if(!RhodesApplication.canStart(paramString))
        {
            Logger.E(TAG, "This is hidden app and can be started only with security key.");
            
            //Toast.makeText(this,"Invalid security token !",Toast.LENGTH_SHORT).show();
            /*
            AlertDialog.Builder b = new AlertDialog.Builder(this);
            b.setCancelable(true);
            b.setOnCancelListener( new DialogInterface.OnCancelListener() {
				public void onCancel(DialogInterface dialog) {
					RhodesService.exit();
				}
			});
            AlertDialog securityAlert = b.create();
            securityAlert.setMessage(RhodesService.getInvalidSecurityTokenMessage());
            securityAlert.setButton("OK", new DialogInterface.OnClickListener(){
				public void onClick(DialogInterface arg0, int arg1) {
					RhodesService.exit();
				}
            	
            });
            securityAlert.show();
            */
            return;
        }

//        String urlStart = uri.getPath();
//        if (urlStart != null) { 
//            if ("".compareTo(urlStart) != 0)
//            {
//                Logger.D(TAG, "PROCESS URL START: " + urlStart);
//                RhoConf.setString("start_path", Uri.decode(urlStart));
//            }
//        }
    }

	public static Context getContext() {
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra == null)
			throw new IllegalStateException("No rhodes activity instance at this moment");
		return ra;
	}
	
	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		RhoBluetoothManager.onActivityResult(requestCode, resultCode, data);
	}
	
}

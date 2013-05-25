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
import java.util.Set;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.security.InvalidParameterException;

import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SimpleMainView;
import com.rhomobile.rhodes.mainview.SplashScreen;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Config;
import com.rhomobile.rhodes.webview.GoogleWebView;

import android.app.Dialog;
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Environment;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;

public class RhodesActivity extends BaseActivity implements SplashScreen.SplashScreenListener {
	
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
	
	private boolean mIsForeground = false;
	private boolean mIsInsideStartStop = false;
	
	public boolean isForegroundNow() {
		return mIsForeground;
	}
	
	public boolean isInsideStartStop() {
		return mIsInsideStartStop;
	}
	
	//------------------------------------------------------------------------------------------------------------------
	// Read SSL settings from config.xml
	
    private ApplicationInfo getAppInfo() {
        String pkgName = getPackageName();
        try {
            ApplicationInfo info = getPackageManager().getApplicationInfo(pkgName, 0);
            return info;
        } catch (NameNotFoundException e) {
            throw new RuntimeException("Internal error: package " + pkgName + " not found: " + e.getMessage());
        }
    }

    private void initConfig(String path) {
        InputStream configIs = null;
        Config config = new Config();
        ApplicationInfo appInfo = getAppInfo();
        try {
            String externalSharedPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/data/" + appInfo.packageName;
            Logger.I(TAG, "Config path: " + path);
            File configXmlFile = new File(path);
            if (configXmlFile.exists()) {
                Logger.I(TAG, "Loading Config.xml from " + configXmlFile.getAbsolutePath());
                configIs = new FileInputStream(configXmlFile);
                config.load(configIs, configXmlFile.getParent());
            }
            else {
                Logger.I(TAG, "Loading Config.xml from resources");
                configIs = getResources().openRawResource(RhoExtManager.getResourceId("raw", "config"));
                config.load(configIs, externalSharedPath);
            }

            String CAFile = config.getValue("CAFile");
            if (CAFile != null && CAFile.length() > 0)
                RhoConf.setString("CAFile", CAFile);

            String CAPath = config.getValue("CAPath");
            if (CAPath != null && CAPath.length() > 0)
                RhoConf.setString("CAPath", CAPath);

        } catch (Throwable e) {
            Logger.E(TAG, "Error loading RhoElements configuraiton ("+e.getClass().getSimpleName()+"): " + e.getMessage());
            Logger.E(TAG, e);
        } finally {
            if (configIs != null) {
                try {
                    configIs.close();
                } catch (IOException e) {
                    // just nothing to do
                }
            }
        }
    }

    private void readRhoElementsConfig() {
        String externalSharedPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/data/" + getAppInfo().packageName;
        String configPath = new File(externalSharedPath, "Config.xml").getAbsolutePath();
        initConfig(configPath);
    }
    
    //------------------------------------------------------------------------------------------------------------------
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        Logger.T(TAG, "onCreate");

        Thread ct = Thread.currentThread();
        //ct.setPriority(Thread.MAX_PRIORITY);
        uiThreadId = ct.getId();

        sInstance = this;

        super.onCreate(savedInstanceState);

		if (!RhodesService.isTitleEnabled()) {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
		}

        requestWindowFeature(Window.FEATURE_PROGRESS);
        getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS);

        mHandler = new Handler();

        Logger.T(TAG, "Creating default main view");
        
        SimpleMainView simpleMainView = new SimpleMainView();
        setMainView(simpleMainView);
        
        Logger.T(TAG, "Creating splash screen");
        
        mSplashScreen = new SplashScreen(this, mMainView, this);
        mMainView = mSplashScreen;

        readRhoElementsConfig();
        RhoExtManager.getImplementationInstance().onCreateActivity(this, getIntent());

        mMainView.setWebView(createWebView(0), -1);

        notifyUiCreated();
        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityCreated);
    }

    public IRhoWebView createWebView(int tabIndex) {
        IRhoWebView view = RhoExtManager.getImplementationInstance().createWebView(tabIndex);
        if (view == null) {
            Logger.T(TAG, "Creating Google web view");
            final GoogleWebView googleWebView = new GoogleWebView(this);
            view = googleWebView;
            RhodesApplication.runWhen(RhodesApplication.AppState.AppStarted, new RhodesApplication.StateHandler(true) {
                @Override
                public void run()
                {
                    googleWebView.applyWebSettings();
                    notifyUiCreated();
                }
            });
        }
        AbsoluteLayout containerView = new AbsoluteLayout(this);
        containerView.addView(view.getView(), new AbsoluteLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT, 0, 0));
        view.setContainerView(containerView);
        view.setWebClient(this);

        return view;
    }

    public MainView switchToSimpleMainView(MainView currentView) {
        IRhoWebView rhoWebView = currentView.detachWebView();
        SimpleMainView view = new SimpleMainView(rhoWebView);
        //rhoWebView.setWebClient(this);
        setMainView(view);
        return view;
    }
	
	public void notifyUiCreated() {
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

        RhoExtManager.getImplementationInstance().onNewIntent(this, intent);
    }

    @Override
    public void onStart() {
        super.onStart();

        Logger.D(TAG, "onStart");
        mIsInsideStartStop = true;

        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityStarted);
        RhoExtManager.getImplementationInstance().onStartActivity(this);
    }

    @Override
    public void onResume() {
        Logger.D(TAG, "onResume");
        mIsForeground = true;
        pauseWebViews(false);
        super.onResume();

        RhoExtManager.getImplementationInstance().onResumeActivity(this);
    }

    @Override
    public void onPause() 
    {
        mIsForeground = false;
        pauseWebViews(true);

        RhoExtManager.getImplementationInstance().onPauseActivity(this);

        super.onPause();
        Logger.D(TAG, "onPause");

        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityPaused);
    }

    private void pauseWebViews( boolean pause ) {
	if ( mMainView != null ) {
	    IRhoWebView wv = mMainView.getWebView(-1);
	    if ( wv != null ) {
		if ( pause ) {
		    wv.onPause();
		} else {
		    wv.onResume();
		}
	    } else {
		for ( int i = 0; i < mMainView.getTabsCount(); ++i ) {
		    wv = mMainView.getWebView(i);
		    if ( wv != null ) {
			if ( pause ) {
			    wv.onPause();
			} else {
			    wv.onResume();
			}
		    }
		}
	    }
    	}
    }

    @Override
    public void onStop() 
    {
        super.onStop();
        Logger.D(TAG, "onStop");

        RhoExtManager.getImplementationInstance().onStopActivity(this);

        mIsInsideStartStop = false;
    }

    @Override
    public void onDestroy() {
        Logger.D(TAG, "onDestroy");

        RhoExtManager.getImplementationInstance().onDestroyActivity(this);

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

    @Override
    public void onSplashScreenGone(SplashScreen splashScreen) {
        ViewGroup parent = (ViewGroup)splashScreen.getSplashView().getParent();
        parent.removeView(splashScreen.getSplashView());
        mMainView = splashScreen.getBackendView();
    }

    @Override
    public void onSplashScreenNavigateBack() {
        moveTaskToBack(true);
    }
    
    @Override
    protected Dialog onCreateDialog(int id/*, Bundle args*/) {
        return RhoExtManager.getImplementationInstance().onCreateDialog(this, id);
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

		//ENABLE_LOADING_INDICATION = !RhoConf.getBool("disable_loading_indication");
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

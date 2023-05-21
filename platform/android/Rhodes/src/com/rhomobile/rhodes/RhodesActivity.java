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

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

//import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.kioskservices.IKioskMode;
import com.rhomobile.rhodes.kioskservices.KioskManager;
import com.rhomobile.rhodes.kioskservices.PermissionManager;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SimpleMainView;
import com.rhomobile.rhodes.mainview.SplashScreen;
import com.rhomobile.rhodes.util.Config;
import com.rhomobile.rhodes.util.Utils;
import com.rhomobile.rhodes.kioskservices.MyOverlayService;
import com.rhomobile.rhodes.util.PerformOnUiThread;


import android.os.Build;
import android.Manifest;
import android.provider.Settings;
import android.os.Environment;



import android.app.Dialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Rect;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Environment;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.widget.FrameLayout;
import android.util.Log;
import android.widget.Button;
import android.app.AlertDialog;
import android.widget.LinearLayout;
import android.widget.ImageView;

import java.util.Vector;

import android.widget.Toast;

import androidx.core.content.PermissionChecker;
import androidx.core.app.ActivityCompat;
import android.app.Activity;


public class RhodesActivity extends BaseActivity implements SplashScreen.SplashScreenListener, ActivityCompat.OnRequestPermissionsResultCallback, IKioskMode {

	private static final String TAG = RhodesActivity.class.getSimpleName();

	private static final boolean DEBUG = false;

	public static boolean IS_WINDOWS_KEY = false;

	public static boolean isShownSplashScreenFirstTime = false;//Used to display the splash screen only once during launch of an application

	public static int MAX_PROGRESS = 10000;

	private static RhodesActivity sInstance = null;

	private Handler mHandler;

	private View mChild;
	private int oldHeight;
	private FrameLayout.LayoutParams frameLayoutParams;
	private int notificationBarHeight = 0;
	public static boolean IS_RESIZE_SIP = false;

	public FrameLayout mTopLayout;
	private SplashScreen mSplashScreen;
	private MainView mMainView = null;
	private static MainView mWebMainView = null;
	private String lastIntent ="android.intent.action.MAIN";
	private RhoMenu mAppMenu;

	private long uiThreadId = 0;
	public static SharedPreferences pref = null;

    private static boolean permissionWindowShow = false;
	private AlertDialog mPermissionsDialog = null;
	private static boolean mIsUseOverlay = false;


	private class AdditionalContentView {
		public View view = null;
		public ViewGroup.LayoutParams lparams = null;
		public AdditionalContentView() {
			view = null;
			lparams = null;
		}
	}


	private static Vector<AdditionalContentView> ourAdditionalContentViews = new Vector<AdditionalContentView>(1);

	@Override
	public void addContentView (View view,
                ViewGroup.LayoutParams params) {
		AdditionalContentView item = new AdditionalContentView();
		item.view = view;
		item.lparams = params;
		if (!(view instanceof SimpleMainView.MyView)) {
			//ourAdditionalContentViews.addElement(item);
		}
		super.addContentView(view, params);
	}


	public void addContentViewInner (View view,
                ViewGroup.LayoutParams params) {
		AdditionalContentView item = new AdditionalContentView();
		item.view = view;
		item.lparams = params;
		if (!(view instanceof SimpleMainView.MyView)) {
			ourAdditionalContentViews.addElement(item);
		}
		super.addContentView(view, params);
	}


	public static FrameLayout onOverlayStarted(FrameLayout overlayLayout) {
		for(int i=0; i < ourAdditionalContentViews.size(); ++i) {
			AdditionalContentView item = ourAdditionalContentViews.get(i);
			ViewGroup vg = (ViewGroup) item.view.getParent();
			if (vg != null) {
				vg.removeView(item.view);
			}
			overlayLayout.addView(item.view, item.lparams);
		}
        return overlayLayout;
	}


	public static interface GestureListener {
		void onTripleTap();
		void onQuadroTap();
	};



	public static class GestureListenerAdapter implements GestureListener {
		public void onTripleTap() {

		}

		public void onQuadroTap() {

		}
	};

	private static ArrayList<GestureListener> ourGestureListeners = new ArrayList<GestureListener>();


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

             String isWindowKey = null;
		try {
		   isWindowKey = config.getValue("isWindowsKey");
		} catch (Exception e) {
		   e.printStackTrace();
	        }
            if (isWindowKey != null && isWindowKey.length() > 0){
            	if(isWindowKey.contains("1")){
            		IS_WINDOWS_KEY = true;
            	}
            }

            String CAFile = config.getValue("CAFile");
            if (CAFile != null && CAFile.length() > 0)
                RhoConf.setString("CAFile", CAFile);

            String CAPath = config.getValue("CAPath");
            if (CAPath != null && CAPath.length() > 0)
                RhoConf.setString("CAPath", CAPath);

            String sVerifyPeer = config.getValue("VerifyPeerCertificate");
            if (sVerifyPeer != null && sVerifyPeer.length() > 0)
                RhoConf.setBoolean("no_ssl_verify_peer", sVerifyPeer.equals("0"));


            String pageZoom=config.getValue("PageZoom");
            if(pageZoom !=null && pageZoom.length()>0)
		RhoConf.setString("PageZoom", pageZoom);
            else
            	RhoConf.setString("PageZoom", "1.0");
            String sFullScreen = null;
			try {
				sFullScreen = config.getValue("FullScreen");
			} catch (Exception e) {
				e.printStackTrace();
			}
            if (sFullScreen != null && sFullScreen.length() > 0){
            	if(sFullScreen.contains("1")){
            		 RhoConf.setBoolean("full_screen", true);
            	}else{
            		 RhoConf.setBoolean("full_screen", false);
            	}
            }




        } catch (Throwable e) {
            Logger.W(TAG, "Error loading RhoElements configuraiton ("+e.getClass().getSimpleName()+"): " + e.getMessage());
            //Logger.W(TAG, e);
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

    private final int RHODES_PERMISSIONS_REQUEST = 1;

    private void requestPermissions() {
        String pkgName = getPackageName();

        try {
            PackageInfo info = getPackageManager().getPackageInfo(pkgName, PackageManager.GET_PERMISSIONS);

            ArrayList<String> requestedPermissions  = new ArrayList<String>();

            for ( String p : info.requestedPermissions ) {

                int result = PermissionChecker.checkSelfPermission( this, p );

                if (result != PackageManager.PERMISSION_GRANTED) {
                    requestedPermissions.add(p);
                }
            }

            if ( requestedPermissions.size() > 0 ) {
                for (String p : requestedPermissions){
                    if (p.contains("MANAGE_EXTERNAL_STORAGE") && 
                        Build.VERSION.SDK_INT >= 30){
                        if (!Environment.isExternalStorageManager()){
                            try{
                                Intent intent = new Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION);
                                intent.addCategory("android.intent.category.DEFAULT");                    
                                intent.setData(Uri.parse(String.format("package:%s", this.getPackageName())));
                                this.startActivityForResult(intent, RHODES_PERMISSIONS_REQUEST);
                            } catch (Exception e) {
                                Intent intent = new Intent();
                                intent.setAction(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION);
                                this.startActivityForResult(intent, RHODES_PERMISSIONS_REQUEST);
                            }
                        }
                    }
                }
                Logger.I(TAG, "Requesting permissions: " + requestedPermissions.toString() );

                ActivityCompat.requestPermissions(
                    this,
                    requestedPermissions.toArray(new String[0]),
                    RHODES_PERMISSIONS_REQUEST);
            }
        } catch (NameNotFoundException e) {
            throw new RuntimeException("Internal error: package " + pkgName + " not found: " + e.getMessage());
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {

        StringBuilder results = new StringBuilder();

        for ( int i = 0; i < permissions.length; ++ i ) {
            results.append( String.format("%s:%d, ", permissions[i], grantResults[i] ) );
        }

        Logger.I(
            TAG,
            String.format(
                "onRequestPermissionsResult code: %d, results: %s",
                    requestCode,
                    results.toString()
            )
        );
    }

	@Override
	protected void onCreate(Bundle savedInstanceState) {
        Logger.T(TAG, "onCreate");

        Thread ct = Thread.currentThread();
        //ct.setPriority(Thread.MAX_PRIORITY);
        uiThreadId = ct.getId();

        sInstance = this;
        pref = sInstance.getApplicationContext().getSharedPreferences("RhodesSharedPreference", RhodesActivity.getContext().MODE_PRIVATE);
        super.onCreate(savedInstanceState);

        if ((RhoConf.isExist("android_title") && !RhoConf.getBool("android_title"))
            || !RhodesService.isTitleEnabled()) {

            requestWindowFeature(Window.FEATURE_NO_TITLE);
        }

        requestWindowFeature(Window.FEATURE_PROGRESS);
        getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS);

        mHandler = new Handler();

        Logger.T(TAG, "Creating default main view");

        mTopLayout = new FrameLayout(this);
        setContentView(mTopLayout);


		if (mWebMainView != null) {
			ViewGroup vg = (ViewGroup) mWebMainView.getView().getParent();
			if (vg != null) {
				vg.removeView(mWebMainView.getView());
			}
			setMainView(mWebMainView);
			isShownSplashScreenFirstTime = true;
			notifyUiCreated();
		}
		else {
			SimpleMainView simpleMainView = new SimpleMainView();
			setMainView(simpleMainView);

			readRhoElementsConfig();
	        RhoExtManager.getImplementationInstance().onCreateActivity(this, getIntent());

	        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityCreated);

		}


        mAppMenu = new RhoMenu();


        if (RhoConf.isExist("resize_sip")) {
			String resizeSIP = RhoConf.getString("resize_sip");
			if (resizeSIP != null && resizeSIP.contains("1")) {
				IS_RESIZE_SIP = true;
				resizeSIP();
			} else {
				IS_RESIZE_SIP = false;
			}
	   }

       requestPermissions();
    }


	public View getContentView() {
		return mTopLayout;
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

	@Override
       protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        Logger.T(TAG, "onNewIntent");
         if(intent !=null && intent.getAction() !=null){
        	 if(((lastIntent.compareTo("android.intent.action.MAIN") == 0) || ! (lastIntent.compareTo("android.intent.action.VIEW") == 0))){
        		 //Use of this If -- generic if to avoid setAction = View for any system related intent. eg:- NFC
        		if(intent.getAction().compareTo("android.intent.action.MAIN") == 0 && (!(lastIntent.compareTo("com.rho.rhoelements.SHORTCUT_ACTION") == 0)) && (RhoExtManager.getInstance().getWebView().getUrl()!=null/*..Double CLick Crash...*/)){
        			//This Else for :- If user click on launch the EB through Shortcut and second time launch the EB through the proper
     	    	   //App that time for handle the Start page , we added this else.
        			 String url = RhoExtManager.getInstance().getWebView().getUrl().toString();
     	        	intent.setAction("android.intent.action.VIEW");
     	        	intent.setData(Uri.parse(url));
        		}
        	 }
        	 else if((intent.getAction().compareTo("android.intent.action.MAIN") == 0) && (!(lastIntent.compareTo("com.rho.rhoelements.SHORTCUT_ACTION") == 0)) && (RhoExtManager.getInstance().getWebView().getUrl()!=null/*..Double CLick Crash...*/) ){
	    	  //This Else is to handle start page when user call  two times on minimize API
        		 //That time last intent will be as VIEW and intent action will be MAIN
	    	    String url = RhoExtManager.getInstance().getWebView().getUrl().toString();
	        	intent.setAction("android.intent.action.VIEW");
	        	intent.setData(Uri.parse(url));
	       }
        }else{
        	//This Else for :- If user Call Restore API that time to maintain the same Page when App resumed back
        	//Because when user click on restore API that time our intent value is NULL.
        	String url = RhoExtManager.getInstance().getWebView().getUrl().toString();
        	intent.setAction("android.intent.action.VIEW");
        	intent.setData(Uri.parse(url));
        }
        handleStartParams(intent);

        RhoExtManager.getImplementationInstance().onNewIntent(this, intent);
        lastIntent = intent.getAction();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        Logger.T(TAG, "onActivityResult");

        //RhoBluetoothManager.onActivityResult(requestCode, resultCode, data);
       // com.rhomobile.rhodes.camera.Camera.onActivityResult(requestCode, resultCode, data);
        try{
            RhoExtManager.getImplementationInstance().onActivityResult(this, requestCode, resultCode, data);
        }catch(Exception e){
            String stackTrace = Log.getStackTraceString(e);
            Logger.W(TAG, e);

            StringBuilder sb = new StringBuilder();
            for (StackTraceElement element : e.getStackTrace()) {
                sb.append(element.toString());
                sb.append("\n");
            }

            Logger.W(TAG, sb.toString());
        };
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);

        RhoExtManager.getImplementationInstance().onSaveInstanceState(outState);
    }

    @Override
    public void onStart() {
        super.onStart();

        Logger.T(TAG, "onStart");
        mIsInsideStartStop = true;

        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityStarted);
        RhoExtManager.getImplementationInstance().onStartActivity(this);
    }

    @Override
    public void onResume() {
        Logger.T(TAG, "onResume");

        if(permissionWindowShow) {
		    startKioskMode(mIsUseOverlay);
		}

        if(!isShownSplashScreenFirstTime){
	        Logger.T(TAG, "Creating splash screen");
	        mSplashScreen = new SplashScreen(this, mMainView, this);
	        mMainView = mSplashScreen;
	        isShownSplashScreenFirstTime = true;
        }

        mIsForeground = true;
        pauseWebViews(false);
        super.onResume();

        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityResumed);
        RhoExtManager.getImplementationInstance().onResumeActivity(this);
    }

    @Override
    public void onPause()
    {
        mIsForeground = false;
        pauseWebViews(true);

        RhoExtManager.getImplementationInstance().onPauseActivity(this);

        super.onPause();
        Logger.T(TAG, "onPause");

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
        Logger.T(TAG, "onStop");

        //RhoExtManager.getImplementationInstance().onStopActivity(this);

        mIsInsideStartStop = false;
        // RhoExtManager.getInstance().quitApp();
        //RhodesService.PerformRealExitInUiThread();
    }

    @Override
    public void onDestroy() {
        Logger.T(TAG, "onDestroy");

        //RhoExtManager.getImplementationInstance().onDestroyActivity(this);

        //RhodesApplication.stateChanged(RhodesApplication.UiState.Undefined);

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

			// If the RhoConf setting of disable_back_button is set to 1, pretend we did something.
			if (RhoConf.isExist("disable_back_button") && RhoConf.getInt("disable_back_button") == 1) {
				Logger.D(TAG, "Back button pressed, but back button is disabled in RhoConfig.");
				return true;
			} else {
				MainView v = r.getMainView();
				v.goBack();//back(v.activeTab());
				return true;
			}
		}

		return super.onKeyDown(keyCode, event);
	}

    public RhoMenu getMenu() {
        return mAppMenu;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        super.onCreateOptionsMenu(menu);

        mAppMenu.enumerateMenu(menu);
        Logger.T(TAG, "onCreateOptionsMenu");

        return mAppMenu.getItemsCount() != 0;
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        super.onPrepareOptionsMenu(menu);

        Logger.T(TAG, "onPrepareOptionsMenu");

        mAppMenu.enumerateMenu(menu);
        return mAppMenu.getItemsCount() != 0;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return mAppMenu.onMenuItemSelected(item);
    }

    @Override
    public void onSplashScreenGone(SplashScreen splashScreen) {
        View splashView = splashScreen.getSplashView();
        if (splashView != null) {
            ViewGroup parent = (ViewGroup)splashView.getParent();
            if (parent != null) {
                parent.removeView(splashView);
            }
        }
        if ((mMainView instanceof SplashScreen) && (mMainView == splashScreen)) {
        	mMainView = splashScreen.getBackendView();
        }
    }

    @Override
    public void onSplashScreenNavigateBack() {
        moveTaskToBack(true);
    }

    @Override
    protected Dialog onCreateDialog(int id/*, Bundle args*/) {
        return RhoExtManager.getImplementationInstance().onCreateDialog(this, id);
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        Logger.T(TAG, "onConfigurationChanged");
        super.onConfigurationChanged(newConfig);
        RhoExtManager.getImplementationInstance().onConfigurationChanged(this, newConfig);
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

	public SplashScreen getRhodesSplashScreen() {
		return mSplashScreen;
	}

    public void setMainView(MainView v) {
        if (v != null) {
            mTopLayout.removeAllViews();
            mMainView = v;
            mTopLayout.addView(v.getView(), new FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT, FrameLayout.LayoutParams.MATCH_PARENT));
			if (v instanceof SimpleMainView) {
				mWebMainView = v;
			}

			for(int i=0; i < ourAdditionalContentViews.size(); ++i) {
				AdditionalContentView item = ourAdditionalContentViews.get(i);
				ViewGroup vg = (ViewGroup) item.view.getParent();
				if (vg != null) {
					vg.removeView(item.view);
				}
				super.addContentView(item.view, item.lparams);
			}
        }
    }

	public MainView getMainView() {
		return mMainView;
	}

	public static MainView extractMainView() {
		RhodesActivity ins = sInstance;
		if (ins != null) {
			if (ins.getMainView() instanceof SimpleMainView) {
				mWebMainView = ins.getMainView();
			}
			ins.mTopLayout.removeAllViews();
		}
		return mWebMainView;
	}

	private static Runnable ourRestartActivityCallback = null;

	public static void setRestartActivityCallback(Runnable r) {
		ourRestartActivityCallback = r;
	}


	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		super.onServiceConnected(name, service);

        Logger.D(TAG, "onServiceConnected: " + name.toShortString());

        handleStartParams(getIntent());

        if(mIsServiceAllreadyExist)
        {
			if (ourRestartActivityCallback != null) {
				ourRestartActivityCallback.run();
			}
            onActivityStarted(this);
            getMainView().navigate(RhodesService.currentLocation(-1), -1);
        }
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

	@Override
	public boolean dispatchKeyEvent(KeyEvent event)
	{
		 IRhoExtManager extManager = RhoExtManager.getInstance();
		 if(extManager.onKey(event.getKeyCode(), event))
		 {
			 return true;
		 }
		 return super.dispatchKeyEvent(event);
	}

	public static Context getContext() {
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra == null)
			throw new IllegalStateException("No rhodes activity instance at this moment");
		return ra;
	}

	public static void addGestureListener(GestureListener listener) {
		ourGestureListeners.add(listener);
		Utils.platformLog(TAG, "$$$ addGestureListener()");
	}

	public static void onTripleTap() {
		Utils.platformLog(TAG, "$$$ onTripleTap()");
		Iterator<GestureListener> i = ourGestureListeners.iterator();
		while (i.hasNext()) {
			i.next().onTripleTap();
		}
	}

	public static void onQuadroTap() {
		Utils.platformLog(TAG, "$$$ onQuadroTap()");
		Iterator<GestureListener> i = ourGestureListeners.iterator();
		while (i.hasNext()) {
			i.next().onQuadroTap();
		}
	}

        public static String getDecodeWav(){
    	        String decodeWavPath = null;
		decodeWavPath =pref.getString("scandecodewavkey", "");
		pref.getString("scandecodewavkey", "");
		return decodeWavPath;
	}

        public static void setDecodeWav(String string){
    	        pref.edit().putString("scandecodewavkey", string).commit();
    }

    public void resizeSIP() {
		FrameLayout mLayout = (FrameLayout) this
				.findViewById(android.R.id.content);
		if(mLayout != null && mLayout.getChildAt(0) != null) {
			mChild = mLayout.getChildAt(0);
			mChild.getViewTreeObserver().addOnGlobalLayoutListener(
					new ViewTreeObserver.OnGlobalLayoutListener() {
						public void onGlobalLayout() {
							resizeChildOfContent();

                            Rect r = new Rect();
                            //r will be populated with the coordinates of your view that area still visible.
                            mChild.getWindowVisibleDisplayFrame(r);

                            RhoExtManager.getInstance().onInputMethod( mChild, false, "", r);

						}
					});
			frameLayoutParams = (FrameLayout.LayoutParams) mChild.getLayoutParams();
		}
	}

	private void resizeChildOfContent() {
		int newHeight = calculateUsedHeight();
		if (newHeight != oldHeight && mChild != null && frameLayoutParams != null) {
			int sipHeight = mChild.getRootView().getHeight();
			int heightDiff = sipHeight - newHeight;
			if (heightDiff > (sipHeight / 4)) {
				// keyboard probably just became visible
				frameLayoutParams.height = sipHeight - heightDiff;
			} else {
				// keyboard probably just became hidden
				frameLayoutParams.height = sipHeight;
			}
		if( !BaseActivity.getFullScreenMode()){
			frameLayoutParams.height = frameLayoutParams.height  - notificationBarHeight;
			if (heightDiff > (sipHeight / 4)) {
				frameLayoutParams.height = frameLayoutParams.height  + notificationBarHeight;
			}
		}
			mChild.requestLayout();
			oldHeight = newHeight;
		}
	}

	private int calculateUsedHeight() {
		Rect r = new Rect();
		mChild.getWindowVisibleDisplayFrame(r);
		notificationBarHeight = r.top;
		return (r.bottom - r.top);
	}

    @Override
    public void startKioskMode(boolean use_overlay) {
		mIsUseOverlay = use_overlay;
        Activity mActivity= (Activity) this;
        Context mContext = getApplicationContext();
		//if (!KioskManager.getKioskModeStatus()) {
	        if(PermissionManager.checkPermissions(mContext, mActivity)){
	            Toast.makeText(mContext, "Kiosk mode started", Toast.LENGTH_SHORT).show();
	            if(permissionWindowShow) permissionWindowShow = false;
				if (mPermissionsDialog != null) {
					try {
						mPermissionsDialog.dismiss();
					}
					catch(Exception ex) {

					}
					mPermissionsDialog = null;
				}
	            KioskManager.setKioskMode(true);
				if (mIsUseOverlay) {
					PerformOnUiThread.exec(new Runnable() {
			             @Override
			             public void run() {
			                 startOverlay();
			             }
			        }, 500);

				}
	        }
	        else{
	            permissionWindowShow = true;
	            showAlertPermission();
	        }
		//}
    }

    @Override
    public void stopKioskMode() {
		if (KioskManager.getKioskModeStatus()) {
	        KioskManager.setKioskMode(false);

	        Context mContext = getApplicationContext();
	        Toast.makeText(mContext, "Kiosk mode finished", Toast.LENGTH_SHORT).show();
			if (mIsUseOverlay) {
				stopOverlay();
			}

			//PermissionManager.setDefaultLauncher(this);
		}
    }

    @Override
    public boolean isKioskMode() {
        return KioskManager.getKioskModeStatus();
    }

    @Override
    public void startOverlay() {
        Activity mActivity= (Activity) this;
        Context mContext = getApplicationContext();
        if(PermissionManager.checkOverlayPermission(mContext)){
            Intent overlay = new Intent(this, MyOverlayService.class);
            startService(overlay);
        }
        else
            PermissionManager.setOverlayPermission(mActivity);
    }

    @Override
    public void stopOverlay() {
        // TODO Auto-generated method stub
		MyOverlayService os = MyOverlayService.getInstance();
		if (os != null) {
			if (MyOverlayService.isOverlayMode()) {
				os.stopOverlayMode();
			}
		}
        //throw new UnsupportedOperationException("Unimplemented method 'stopOverlay'");
    }

    @Override
    public boolean isAllPermissions() {
        Activity mActivity= (Activity) this;
        Context mContext = getApplicationContext();
        boolean perm = PermissionManager.checkPermissions(mContext, mActivity);
        //Toast.makeText(mActivity, "Permission: "+ (perm? "true" : "flase"),Toast.LENGTH_LONG).show();
        return PermissionManager.checkPermissions(mContext, mActivity);
    }

    @Override
    public void showPermissionsList() {
        showAlertPermission();
    }

    private Activity mActivity = null;

    private void showAlertPermission() {
        mActivity= (Activity) this;

        AlertDialog.Builder adb = new AlertDialog.Builder(this);
        adb.setTitle("Permission check")
            .setCancelable(false);

        LinearLayout view = (LinearLayout) getLayoutInflater().inflate(R.layout.perrmission_alert_dialog, null);
        adb.setView(view);

		if (mPermissionsDialog != null) {
			try {
				mPermissionsDialog.dismiss();
			}
			catch(Exception ex) {

			}
			mPermissionsDialog = null;
		}

        mPermissionsDialog = adb.create();


        //===================== ACCESSIBILITY SERVICE =====================

        Button asBtn = view.findViewById(R.id.accessibilityServiceBtn);
        ImageView asImg = view.findViewById(R.id.accessibilityServiceStatus);

        asBtn.setVisibility(
            (!PermissionManager.checkAccessibilityServicePermission(this)?View.VISIBLE:View.GONE)
        );
        asImg.setVisibility(
            (PermissionManager.checkAccessibilityServicePermission(this)?View.VISIBLE:View.GONE)
        );

        asBtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                PermissionManager.setAccessibilityServicePermission(mActivity);
                //mPermissionsDialog.cancel();
            }
        });

        //===================== NOTIFICATION SERVICE =====================

        Button nBtn = view.findViewById(R.id.notificationServiceBtn);
        ImageView nImg = view.findViewById(R.id.notificationServiceStatus);

        nBtn.setVisibility(
            (!PermissionManager.checkNotificationServicePermission(this, this)?View.VISIBLE:View.GONE)
        );
        nImg.setVisibility(
            (PermissionManager.checkNotificationServicePermission(this, this)?View.VISIBLE:View.GONE)
        );

        nBtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                PermissionManager.setNotificationServicePermission(mActivity);
                //mPermissionsDialog.cancel();
            }
        });

        //===================== DEFAULT LAUNCHER =====================

        Button dlBtn = view.findViewById(R.id.defaultLauncherBtn);
        ImageView dlImg = view.findViewById(R.id.defaultLauncherStatus);

        dlBtn.setVisibility(
            (!PermissionManager.isMyLauncherDefault(this)?View.VISIBLE:View.GONE)
        );
        dlImg.setVisibility(
            (PermissionManager.isMyLauncherDefault(this)?View.VISIBLE:View.GONE)
        );

        dlBtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                PermissionManager.setDefaultLauncher(mActivity);
                //mPermissionsDialog.cancel();
            }
        });

        //===================== CALL PHONE =====================

        Button cpBtn = view.findViewById(R.id.callPhoneBtn);
        ImageView cpImg = view.findViewById(R.id.callPhoneStatus);

        cpBtn.setVisibility(
            (!PermissionManager.checkCallPhonePermission(this)?View.VISIBLE:View.GONE)
        );
        cpImg.setVisibility(
            (PermissionManager.checkCallPhonePermission(this)?View.VISIBLE:View.GONE)
        );

        cpBtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                PermissionManager.setCallPhonePermission(mActivity);
                //mPermissionsDialog.cancel();
            }
        });

        //===================== OVERLAY PERMISSION =====================

        Button oBtn = view.findViewById(R.id.overlayServiceBtn);
        ImageView oImg = view.findViewById(R.id.overlayServiceStatus);

        oBtn.setVisibility(
            (!PermissionManager.checkOverlayPermission(this)?View.VISIBLE:View.GONE)
        );
        oImg.setVisibility(
            (PermissionManager.checkOverlayPermission(this)?View.VISIBLE:View.GONE)
        );

        oBtn.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View v) {
                PermissionManager.setOverlayPermission(mActivity);
                //mPermissionsDialog.cancel();
            }
        });

		try {
        	mPermissionsDialog.show();
		}
		catch(Exception e) {

		}
    }

	public Context getCurrentContextForUI() {
		//MyOverlayService os = MyOverlayService.getInstance();
		//if (os != null) {
		//	if (MyOverlayService.isOverlayMode()) {
		//		return os;
		//	}
		//}
		return this;
	}

}

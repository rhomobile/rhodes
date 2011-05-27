package com.rhomobile.rhodes;

import java.lang.reflect.Constructor;
import java.util.ArrayList;
import java.util.Iterator;

import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.webview.ChromeClientOld;
import com.rhomobile.rhodes.webview.RhoWebSettings;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class RhodesActivity extends BaseActivity {
	
	private static final String TAG = RhodesActivity.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	private static final boolean USE_DELAYED_MAINVIEW_DISPLAY = false;
	
	public static boolean ENABLE_LOADING_INDICATION = true;
	
	public static int MAX_PROGRESS = 10000;
	
	static final String RHO_START_PARAMS_KEY = "RhoStartParams";
	static final String RHO_URL_START_KEY = "RhoUrlStart";
	
	private static RhodesActivity sInstance = null;
	
	private Handler mHandler;
	
	private SplashScreen mSplashScreen;
	
	private MainView mMainView;
	
	private RhoMenu mAppMenu;
	
	private WebChromeClient mChromeClient;
	private WebViewClient mWebViewClient;
	private RhoWebSettings mWebSettings;

	
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
	
	
	private Runnable mSetup = new Runnable() {
		public void run() {
			doSetup();
		}
	};
	
	
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
		
		super.onCreate(savedInstanceState);
		
		mListeners = new ArrayList<RhodesActivityListener>();

		Thread ct = Thread.currentThread();
		//ct.setPriority(Thread.MAX_PRIORITY);
		uiThreadId = ct.getId();

		if (!RhodesService.isTitleEnabled()) {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
		}
		else {
		}
		requestWindowFeature(Window.FEATURE_PROGRESS);
		getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 10000);

		mSplashScreen = new SplashScreen(this);
		setMainView(mSplashScreen);
		
		mHandler = new Handler();
		mHandler.post(mSetup);
		
		sInstance = this;

		Log.i(TAG, ">>>>>>>>>>>>>>> onCreate()");

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
	
	private void notifyUiCreated() {
		RhodesService r = RhodesService.getInstance();
		if ( r != null ) {
			r.callUiCreatedCallback();
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
				
					r.callUiCreatedCallback();
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
        Log.d(TAG, "RhodesActivity.onNewIntent()");
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
	    RhodesService.rhodesActivityStarted(true);

        Log.d(TAG, "RhodesActivity.onStart()");
        mIsInsideStartStop = true;
        
        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityStarted);
	}
	
	@Override
	public void onResume() {
        Log.d(TAG, "RhodesActivity.onResume()");
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
    	
        RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityPaused);

        RhodesService.rhodesActivityStarted(false);

        super.onPause();
        Log.d(TAG, "RhodesActivity.onPause()");

        RhodesService.callUiDestroyedCallback();
    }

    @Override
	public void onStop() 
	{
		super.onStop();
        Log.d(TAG, "RhodesActivity.onStop()");
        mIsInsideStartStop = false;
	}
	
	@Override
	public void onDestroy() {
        Log.d(TAG, "RhodesActivity.onDestroy()");
        
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
				Log.d(TAG, "onKeyDown: r=" + r);
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
	
	public void post(Runnable r, int delay) {
		mHandler.postDelayed(r, delay);
	}
	
	private void doSetup() {
		initWebStuff();
	}
	
	public SplashScreen getSplashScreen() {
		return mSplashScreen;
	}
	
	public void setMainView(MainView v) {
		setMainView(v, true);
	}
	
	public void setMainView(final MainView v, boolean waitUntilNavigationDone) {
		if (DEBUG)
			Log.d(TAG, "setMainView: v=" + v + "; mMainView=" + mMainView);
		
		// If there's no previous mMainView, don't wait
		if (mMainView == null)
			waitUntilNavigationDone = false;
		
		// Set mMainView right now but not yet do it visible
		mMainView = v;

		// This is action need to be executed when mMainView should become visible 
		final Runnable setMainViewVisible = new Runnable() {
			public void run() {
				if (DEBUG)
					Log.d(TAG, "setMainViewAction: v=" + v);
				setContentView(v.getView());
			}
		};
		
		if (!USE_DELAYED_MAINVIEW_DISPLAY /* || !waitUntilNavigationDone*/) {
			// Make new MainView visible right now
			setMainViewVisible.run();
		}
		else {
			// If we're requested to wait until first navigation will be done,
			// use the trick: keep current main view until first navigate will be
			// finished in the new MainView.
			// This will end up in good user experience - user will see
			// new MainView only when it will have completely load its content
			// (no blank screens for user).
			WebView webView = v.getWebView(0);
			webView.setWebViewClient(new WebViewClient() {
				@Override
				public void onPageFinished(WebView view, String url) {
					mWebViewClient.onPageFinished(view, url);
					// Restore standard WebViewClient to be sure this callback will not
					// be called anymore (it should be called only once)
					view.setWebViewClient(mWebViewClient);
					
					setMainViewVisible.run();
				}
			});
		}
	}
	
	public MainView getMainView() {
		return mMainView;
	}
	
	public WebView createWebView() {
		WebView view = new WebView(this);
		mWebSettings.setWebSettings(view);
		view.setWebChromeClient(mChromeClient);
		view.setWebViewClient(mWebViewClient);
		view.clearCache(true);
		
		return view;
	}
	
	private void initWebStuff() {
		String ccName;
		String wsName;
		int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
		if (sdkVersion < Build.VERSION_CODES.ECLAIR_MR1) {
			ccName = "ChromeClientOld";
			wsName = "RhoWebSettingsOld";
		}
		else {
			ccName = "ChromeClientNew";
			wsName = "RhoWebSettingsNew";
		}
		
		try {
			String pkgname = ChromeClientOld.class.getPackage().getName();
			String fullName = pkgname + "." + ccName;
			Class<? extends WebChromeClient> ccClass =
				Class.forName(fullName).asSubclass(WebChromeClient.class);
			
			Constructor<? extends WebChromeClient> ctor = ccClass.getConstructor(RhodesActivity.class);
			mChromeClient = ctor.newInstance(this);
			
			pkgname = RhoWebSettings.class.getPackage().getName();
			fullName = pkgname + "." + wsName;
			Class<? extends RhoWebSettings> wsClass =
				Class.forName(fullName).asSubclass(RhoWebSettings.class);
			mWebSettings = wsClass.newInstance();
			
			mWebViewClient = new WebViewClient() {
				@Override
				public boolean shouldOverrideUrlLoading(WebView view, String url) {
					return getService().handleUrlLoading(url);
				}
				
				@Override
				public void onPageStarted(WebView view, String url, Bitmap favicon) {
					if (ENABLE_LOADING_INDICATION)
						getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0);
					super.onPageStarted(view, url, favicon);
				}
				
				@Override
				public void onPageFinished(WebView view, String url) {
					// Set title
					String title = view.getTitle();
					setTitle(title);
					if (ENABLE_LOADING_INDICATION)
						getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS);
					
					super.onPageFinished(view, url);
				}
			};
		}
		catch (Exception e) {
			throw new IllegalStateException(e);
		}
	}

	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		super.onServiceConnected(name, service);

        Logger.D(TAG, "onServiceConnected: " + name.toShortString());

        Intent intent = getIntent();
        String startParams = (intent.getData() != null) ? intent.toUri(0) : "";
        Uri uri = Uri.parse(startParams);
        String scheme = uri.getScheme();
        if(startParams.compareTo("") != 0)
        {
            startParams = startParams.substring(scheme.length() + 1);
            if(startParams.startsWith("//"))
                startParams = startParams.substring(2);
        }

        if(!RhodesApplication.canStart(startParams))
        {
            Logger.E(TAG, "This is hidden app and can be started only with security key.");
            RhodesService.exit();
            return;
        }

        String urlStart = uri.getPath();
        if (urlStart.compareTo("") != 0)
        {
            Logger.D(TAG, "PROCESS URL START: " + urlStart);
            RhoConf.setString("start_path", Uri.decode(urlStart));
        }

		ENABLE_LOADING_INDICATION = !RhoConf.getBool("disable_loading_indication");
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

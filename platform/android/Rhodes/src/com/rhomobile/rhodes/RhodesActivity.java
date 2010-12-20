package com.rhomobile.rhodes;

import java.lang.reflect.Constructor;

import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.webview.ChromeClientOld;
import com.rhomobile.rhodes.webview.RhoWebSettings;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Window;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class RhodesActivity extends BaseActivity implements ServiceConnection {
	
	private static final String TAG = RhodesActivity.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	public static boolean ENABLE_LOADING_INDICATION = true;
	
	public static int MAX_PROGRESS = 10000;
	
	static final String RHO_START_PARAMS_KEY = "RhoStartParams";
	
	private static RhodesActivity sInstance;
	
	private RhodesService mRhodesService;
	private boolean mBoundToService;
	
	private Handler mHandler;
	
	private WebChromeClient mChromeClient;
	private WebViewClient mWebViewClient;
	private RhoWebSettings mWebSettings;
	
	private SplashScreen mSplashScreen;
	
	private long uiThreadId = 0;
	
	public long getUiThreadId() {
		return uiThreadId;
	}
	
	private Runnable mSetup = new Runnable() {
		public void run() {
			doSetup();
		}
	};
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		Thread ct = Thread.currentThread();
		//ct.setPriority(Thread.MAX_PRIORITY);
		uiThreadId = ct.getId();
		
		mSplashScreen = new SplashScreen(this);
		setContentView(mSplashScreen.getContentView());
		
		Intent intent = new Intent(getApplicationContext(), RhodesService.class);
		bindService(intent, this, Context.BIND_AUTO_CREATE);
		mBoundToService = true;
		
		mHandler = new Handler();
		mHandler.post(mSetup);
	}

	@Override
	public void onStart() {
		super.onStart();
		sInstance = this;
	}
	
	@Override
	public void onStop() {
		sInstance = null;
		super.onStop();
	}
	
	@Override
	public void onDestroy() {
		if (mBoundToService) {
			unbindService(this);
			mBoundToService = false;
		}
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
			v.back(v.activeTab());
			return true;
		}
		
		return super.onKeyDown(keyCode, event);
	}
	
	public static RhodesActivity getInstance() {
		return sInstance;
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
		//setupMainView();
	}
	
	public WebView createWebView() {
		WebView view = new WebView(this);
		mWebSettings.setWebSettings(view);
		view.setWebChromeClient(mChromeClient);
		view.setWebViewClient(mWebViewClient);
		view.clearCache(true);
		
		return view;
	}
	
	private void setupMainView() {
		if (DEBUG)
			Log.d(TAG, "setupMainView");
		WebView view = createWebView();
		// Set another web view client for one time
		// It will be replace by the standard one as soon as
		// first page will be loaded
		view.setWebViewClient(new WebViewClient() {
			@Override
			public void onPageFinished(WebView view, String url) {
				mWebViewClient.onPageFinished(view, url);
				view.setWebViewClient(mWebViewClient);
				
				setContentView(view);
			}
		});
		
		view.loadUrl("http://www.google.ru/");
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
		Log.d(TAG, "Connected to service");
		mRhodesService = ((RhodesService.LocalBinder)service).getService();
		
		if (!isValidSecurityToken()) {
			Logger.E(TAG, "SECURITY_TOKEN parameter is not valid for this application !");
			getRhodesApplication().exit();
			return;
		}
		
		ENABLE_LOADING_INDICATION = !RhoConf.getBool("disable_loading_indication");
	}

	@Override
	public void onServiceDisconnected(ComponentName name) {
		Log.d(TAG, "Disconnected from service");
		mRhodesService = null;
	}
	
	private boolean isValidSecurityToken() {
		boolean valid = true;
		String security_token = RhodesService.getBuildConfig("security_token");
		if (security_token != null) {
			if (security_token.length() > 0) {
				valid = false;
				Object params = getIntent().getExtras();
				if (params != null && params instanceof Bundle) {
					Bundle startParams = (Bundle)params;
					String rho_start_params = startParams.getString(RHO_START_PARAMS_KEY);
					if (rho_start_params != null) {
						String security_token_key = "sequrity_token=";
						int sec_index = rho_start_params.indexOf(security_token_key);
						if (sec_index >= 0) {
							String tmp = rho_start_params.substring(sec_index + security_token_key.length(), rho_start_params.length() - sec_index - security_token_key.length());
							int end_of_token = tmp.indexOf(",");
							if (end_of_token >= 0) {
								tmp = tmp.substring(0, end_of_token);
							}
							end_of_token = tmp.indexOf(" ");
							if (end_of_token >= 0) {
								tmp = tmp.substring(0, end_of_token);
							}
							if (tmp.equals(security_token)) {
								valid = true;
							}
						}
					}
				}
			}
		}
		return valid;
	}

}

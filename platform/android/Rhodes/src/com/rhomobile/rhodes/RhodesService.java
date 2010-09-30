package com.rhomobile.rhodes;

import java.io.File;
import java.io.IOException;
import java.util.Calendar;
import java.util.Enumeration;
import java.util.Locale;
import java.util.TimeZone;
import java.util.Vector;

import com.rhomobile.rhodes.bluetooth.RhoBluetoothManager;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.geolocation.GeoLocation;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.ui.AboutDialog;
import com.rhomobile.rhodes.ui.LogOptionsDialog;
import com.rhomobile.rhodes.ui.LogViewDialog;
import com.rhomobile.rhodes.uri.MailUriHandler;
import com.rhomobile.rhodes.uri.SmsUriHandler;
import com.rhomobile.rhodes.uri.TelUriHandler;
import com.rhomobile.rhodes.uri.UriHandler;
import com.rhomobile.rhodes.uri.VideoUriHandler;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.webview.ChromeClientOld;
import com.rhomobile.rhodes.webview.RhoWebSettings;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Build;
import android.os.Handler;
import android.os.PowerManager;
import android.os.Process;
import android.os.PowerManager.WakeLock;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class RhodesService {
	
	private final static String TAG = "RhodesService";

	public static final String INTENT_EXTRA_PREFIX = "com.rhomobile.rhodes.";
	
	public static final int RHO_SPLASH_VIEW = 1;
	public static final int RHO_MAIN_VIEW = 2;
	public static final int RHO_TOOLBAR_VIEW = 3;
	
	private static RhodesService instance = null;
	
	public static RhodesService getInstance() {
		return instance;
	}
	
	private Activity ctx;
	
	public Context getContext() {
		return ctx;
	}
	
	public Activity getMainActivity() {
		return ctx;
	}
	
	private RhoLogConf m_rhoLogConf = new RhoLogConf();
	public RhoLogConf getLogConf() {
		return m_rhoLogConf;
	}
	
	private boolean needGeoLocationRestart = false;
	
	private int activitiesActive = 0;
	
	public static int WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FULLSCREEN;
	public static int WINDOW_MASK = WindowManager.LayoutParams.FLAG_FULLSCREEN;
	
	public static boolean ENABLE_LOADING_INDICATION = true;
	
	public static int MAX_PROGRESS = 10000;
	
	private long uiThreadId = 0;
	
	public long getUiThreadId() {
		return uiThreadId;
	}
	
	public void setInfo(Activity c, long id, Handler handler) {
		ctx = c;
		uiThreadId = id;
		uiHandler = handler;
		RhoBluetoothManager.sharedInstance();
	}
	
	private Handler uiHandler;
	
	public void post(Runnable r) {
		uiHandler.post(r);
	}
	
	private static int screenWidth;
	private static int screenHeight;
	private static int screenOrientation;
	
	private static float screenPpiX;
	private static float screenPpiY;
	
	private static boolean isCameraAvailable;
	
	private WebChromeClient chromeClient;
	private RhoWebSettings webSettings;
	
	private SplashScreen splashScreen = null;
	
	private ViewGroup outerFrame = null;
	private MainView mainView;
	
	private native void initClassLoader(ClassLoader c);
	
	private native void createRhodesApp();
	private native void startRhodesApp();
	
	public native void doSyncAllSources(boolean v);
	
	public native String getOptionsUrl();
	public native String getStartUrl();
	public native String getCurrentUrl();
	public native String getAppBackUrl();
	
	public static native String getBlobPath();
	
	public native String normalizeUrl(String url);
	
	public static native void loadUrl(String url);
	
	public static native void navigateBack();
	
	public native void doRequest(String url);
	
	public native void callActivationCallback(boolean active);
	
	public native static void makeLink(String src, String dst);
	
	public static native void onScreenOrientationChanged(int width, int height, int angle);
	
	private String rootPath = null;
	private native void nativeInitPath(String rootPath, String sqliteJournalsPath, String apkPath);
	
	static PowerManager.WakeLock wakeLockObject = null;
	static boolean wakeLockEnabled = false;
	
	public static int rho_sys_set_sleeping(int enable) {
		Logger.I(TAG, "rho_sys_set_sleeping("+enable+")");
		int wasEnabled = 1;
		if (wakeLockObject != null) {
			wasEnabled = 0;
		}
		if (enable != 0) {
			// disable lock device
			PerformOnUiThread.exec( new Runnable() {
				public void run() {
					if (wakeLockObject != null) {
						wakeLockObject.release();
						wakeLockObject = null;
						wakeLockEnabled = false;
					}
				}
			}, false);
		}
		else {
			// lock device from sleep
			PerformOnUiThread.exec( new Runnable() {
				public void run() {
					if (wakeLockObject == null) {
						PowerManager pm = (PowerManager)getInstance().getContext().getSystemService(Context.POWER_SERVICE);
						if (pm != null) {
							wakeLockObject = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
							wakeLockObject.acquire();
							wakeLockEnabled = true;
						}
						else {
							Logger.E(TAG, "rho_sys_set_sleeping() - Can not get PowerManager !!!");
						}
					}
				}
			}, false);
		}
		return wasEnabled;
	}
	
	private void initRootPath() {
		ApplicationInfo appInfo = getAppInfo();
		String dataDir = appInfo.dataDir;
		
		rootPath = dataDir + "/rhodata/";
		Log.d(TAG, "Root path: " + rootPath);
		
		String sqliteJournalsPath = dataDir + "/sqlite_stmt_journals/";
		Log.d(TAG, "Sqlite journals path: " + sqliteJournalsPath);
		
		File f = new File(rootPath);
		f.mkdirs();
		f = new File(f, "db/db-files");
		f.mkdirs();
		f = new File(sqliteJournalsPath);
		f.mkdirs();
		
		String apkPath = appInfo.sourceDir;
		
		nativeInitPath(rootPath, sqliteJournalsPath, apkPath);
	}
	
	private ApplicationInfo getAppInfo() {
		String pkgName = ctx.getPackageName();
		try {
			ApplicationInfo info = ctx.getPackageManager().getApplicationInfo(pkgName, 0);
			return info;
		} catch (NameNotFoundException e) {
			throw new RuntimeException("Internal error: package " + pkgName + " not found: " + e.getMessage());
		}
	}
	
	public String getRootPath() {
		return rootPath;
	}
	
	private Vector<UriHandler> uriHandlers = new Vector<UriHandler>();
	
	private boolean handleUrlLoading(String url) {
		Enumeration<UriHandler> e = uriHandlers.elements();
		while (e.hasMoreElements()) {
			UriHandler handler = e.nextElement();
			try {
				if (handler.handle(url))
					return true;
			}
			catch (Exception ex) {
				Logger.E(TAG, ex.getMessage());
				continue;
			}
		}
		
		return false;
	}
	
	private void showSplashScreen() {
		splashScreen = new SplashScreen(ctx);
		splashScreen.start(outerFrame);
	}
	
	public void hideSplashScreen() {
		PerformOnUiThread.exec(new Runnable() {
					public void run() {
		if (splashScreen != null) {
			splashScreen.hide(outerFrame);
			splashScreen = null;
		}
		View view = mainView.getView();
		view.setVisibility(View.VISIBLE);
		view.requestFocus();
		}
				}, false);
	}
	
	public WebView createWebView() {
		WebView w = new WebView(ctx);
		
		webSettings.setWebSettings(w);
		
		w.clearCache(true);

		w.setWebViewClient(new WebViewClient() {
			
			private boolean splashHidden = false;

			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				return handleUrlLoading(url);
			}
			
			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				Rhodes r = Rhodes.getInstance();
				if (ENABLE_LOADING_INDICATION)
					r.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, 0);
				super.onPageStarted(view, url, favicon);
			}
			
			@Override
			public void onPageFinished(WebView view, String url) {
				// Set title
				Rhodes r = Rhodes.getInstance();
				String title = view.getTitle();
				r.setTitle(title);
				// Hide splash screen
				if (!splashHidden && url.startsWith("http://")) {
					hideSplashScreen();
					splashHidden = true;
				}
				if (ENABLE_LOADING_INDICATION)
					r.getWindow().setFeatureInt(Window.FEATURE_PROGRESS, MAX_PROGRESS);
				super.onPageFinished(view, url);
			}

		});
		
		w.setWebChromeClient(chromeClient);
		
		return w;
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
			chromeClient = ccClass.newInstance();
			
			pkgname = RhoWebSettings.class.getPackage().getName();
			fullName = pkgname + "." + wsName;
			Class<? extends RhoWebSettings> wsClass =
				Class.forName(fullName).asSubclass(RhoWebSettings.class);
			webSettings = wsClass.newInstance();
		}
		catch (Exception e) {
			throw new IllegalStateException(e);
		}
	}
	
	public void setMainView(MainView v) {
		View main = outerFrame.findViewById(RHO_MAIN_VIEW);
		if (main != null)
			outerFrame.removeView(main);
		mainView = v;
		main = mainView.getView();
		if (outerFrame.findViewById(RHO_SPLASH_VIEW) != null)
			main.setVisibility(View.INVISIBLE);
		outerFrame.addView(main);
	}
	
	public MainView getMainView() {
		return mainView;
	}
	
	public void setRootWindow(ViewGroup rootWindow) {
		if (rootWindow == outerFrame)
			return;
		if (outerFrame != null)
			outerFrame.removeAllViews();
		outerFrame = rootWindow;
		outerFrame.addView(mainView.getView());
	}
	
	public static boolean isCreated() {
		return instance != null;
	}
	
	public RhodesService(Activity c, ViewGroup rootWindow) {
		ctx = c;
		instance = this;
		
		initClassLoader(ctx.getClassLoader());

		try {
			initRootPath();
			RhoFileApi.init();
		} catch (IOException e) {
			Log.e(TAG, e.getMessage());
			exitApp();
			return;
		}
		
		if (Utils.isAppHashChanged()) {
			try {
				Log.i(TAG, "Application hash was changed, so remove files");
				Utils.deleteRecursively(new File(rootPath));
				initRootPath();
				RhoFileApi.init();
				RhoFileApi.copy("hash");
			} catch (IOException e) {
				Log.e(TAG, e.getMessage());
				exitApp();
				return;
			}
		}
		
		createRhodesApp();
		
		boolean fullScreen = true;
		if (RhoConf.isExist("full_screen"))
			fullScreen = RhoConf.getBool("full_screen");
		if (!fullScreen) {
			WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
			WINDOW_MASK = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
		}
		
		ENABLE_LOADING_INDICATION = !RhoConf.getBool("disable_loading_indication");
		
		initWebStuff();
		
		outerFrame = rootWindow;
		
		Logger.I("Rhodes", "Loading...");
		showSplashScreen();
		
		// Increase WebView rendering priority
		WebView w = new WebView(ctx);
		WebSettings webSettings = w.getSettings();
		webSettings.setRenderPriority(WebSettings.RenderPriority.HIGH);
		
		// Get screen width/height
		WindowManager wm = (WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE);
		Display d = wm.getDefaultDisplay();
		screenHeight = d.getHeight();
		screenWidth = d.getWidth();
		screenOrientation = d.getOrientation();
		
		DisplayMetrics metrics = new DisplayMetrics();
		d.getMetrics(metrics);
		screenPpiX = metrics.xdpi;
		screenPpiY = metrics.ydpi;
		
		// TODO: detect camera availability
		isCameraAvailable = true;
		
		// Register custom uri handlers here
		uriHandlers.addElement(new MailUriHandler(ctx));
		uriHandlers.addElement(new TelUriHandler(ctx));
		uriHandlers.addElement(new SmsUriHandler(ctx));
		uriHandlers.addElement(new VideoUriHandler(ctx));
		
		Thread init = new Thread(new Runnable() {

			public void run() {
				startRhodesApp();
			}
			
		});
		init.start();
	}
	
	public void startActivity(Intent intent) {
		ctx.startActivity(intent);
	}

	public void exitApp() {
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
				if (wakeLockObject != null) {
					wakeLockObject.release();
					wakeLockObject = null;
					wakeLockEnabled = false;
				}
			}
		}, false);
		Process.killProcess(Process.myPid());
	}
	
	public void activityStarted() {
		if (activitiesActive == 0) {
			if (needGeoLocationRestart) {
				GeoLocation.isKnownPosition();
				needGeoLocationRestart = false;
			}
			PerformOnUiThread.exec( new Runnable() {
				public void run() {
					if (wakeLockEnabled) {
						if (wakeLockObject == null) {
							PowerManager pm = (PowerManager)getInstance().getContext().getSystemService(Context.POWER_SERVICE);
							if (pm != null) {
								Logger.I(TAG, "activityStarted() restore wakeLock object");
								wakeLockObject = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
								wakeLockObject.acquire();
							}
							else {
								Logger.E(TAG, "activityStarted() - Can not get PowerManager !!!");
							}
						}
					}
				}
			}, false);
			callActivationCallback(true);
		}
		++activitiesActive;
	}
	
	public void activityStopped() {
		PerformOnUiThread.exec( new Runnable() {
			public void run() {
				if (wakeLockObject != null) {
					Logger.I(TAG, "activityStopped() temporary destroy wakeLock object");
					wakeLockObject.release();
					wakeLockObject = null;
				}
			}
		}, false);
		--activitiesActive;
		if (activitiesActive == 0) {
			needGeoLocationRestart = GeoLocation.isAvailable();
			GeoLocation.stop();
			callActivationCallback(false);
		}
	}
	
	public void rereadScreenProperties() {
		// check for orientarion changed
		// Get screen width/height
		WindowManager wm = (WindowManager)ctx.getSystemService(Context.WINDOW_SERVICE);
		Display d = wm.getDefaultDisplay();
		screenHeight = d.getHeight();
		screenWidth = d.getWidth();
		int newScreenOrientation = d.getOrientation();
		if (newScreenOrientation != screenOrientation) {
			onScreenOrientationChanged(screenWidth, screenHeight, 90);
			screenOrientation = newScreenOrientation; 				
		}
	}
	
	public static void showAboutDialog() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final AboutDialog aboutDialog = new AboutDialog(Rhodes.getInstance());
				aboutDialog.setTitle("About");
				aboutDialog.setCanceledOnTouchOutside(true);
				aboutDialog.setCancelable(true);
				aboutDialog.show();
			}
		}, false);
	}
	
	public static void showLogView() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final LogViewDialog logViewDialog = new LogViewDialog(Rhodes.getInstance());
				logViewDialog.setTitle("Log View");
				logViewDialog.setCancelable(true);
				logViewDialog.show();
			}
		}, false);
	}
	
	public static void showLogOptions() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final LogOptionsDialog logOptionsDialog = new LogOptionsDialog(Rhodes.getInstance());
				logOptionsDialog.setTitle("Logging Options");
				logOptionsDialog.setCancelable(true);
				logOptionsDialog.show();
			}
		}, false);
	}
	
	// Called from native code
	public static void deleteFilesInFolder(String folder) {
		try {
			String[] children = new File(folder).list();
			for (int i = 0; i != children.length; ++i)
				Utils.deleteRecursively(new File(folder, children[i]));
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
	
	private static boolean hasNetwork() {
		if (!Capabilities.NETWORK_STATE_ENABLED) {
			Logger.E(TAG, "HAS_NETWORK: Capability NETWORK_STATE disabled");
			return false;
		}
		
		Context ctx = RhodesService.getInstance().getContext();
		ConnectivityManager conn = (ConnectivityManager)ctx.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (conn == null)
		{
			Logger.E(TAG, "HAS_NETWORK: cannot create ConnectivityManager");
			return false;
		}
		 
		NetworkInfo[] info = conn.getAllNetworkInfo();
		if (info == null)
		{
			Logger.E(TAG, "HAS_NETWORK: cannot issue getAllNetworkInfo");
			return false;
		}
		
		for (int i = 0, lim = info.length; i < lim; ++i) 
		{
		    //Logger.I(TAG, "HAS_NETWORK: " + info[i].toString() );
			if (info[i].getState() == NetworkInfo.State.CONNECTED)
				return true;
		}

    	Logger.I(TAG, "HAS_NETWORK: all networks are disconnected");
		
		return false;
	}
	
	private static String getCurrentLocale() {
		String locale = Locale.getDefault().getLanguage();
		if (locale.length() == 0)
			locale = "en";
		return locale;
	}
	
	private static String getCurrentCountry() {
		String cl = Locale.getDefault().getCountry();
		return cl;
	}
	
	public static int getScreenWidth() {
		return screenWidth;
	}
	
	public static int getScreenHeight() {
		return screenHeight;
	}
	
	public static Object getProperty(String name) {
		if (name.equalsIgnoreCase("platform"))
			return "ANDROID";
		else if (name.equalsIgnoreCase("locale"))
			return getCurrentLocale();
		else if (name.equalsIgnoreCase("country"))
			return getCurrentCountry();
		else if (name.equalsIgnoreCase("screen_width"))
			return new Integer(getScreenWidth());
		else if (name.equalsIgnoreCase("screen_height"))
			return new Integer(getScreenHeight());
		else if (name.equalsIgnoreCase("has_camera"))
			return new Boolean(isCameraAvailable);
		else if (name.equalsIgnoreCase("has_network"))
			return hasNetwork();
		else if (name.equalsIgnoreCase("ppi_x"))
			return new Float(screenPpiX);
		else if (name.equalsIgnoreCase("ppi_y"))
			return new Float(screenPpiY);
		else if (name.equalsIgnoreCase("phone_number")) {
			TelephonyManager manager = (TelephonyManager)RhodesService.getInstance().
				getContext().getSystemService(Context.TELEPHONY_SERVICE);
			String number = manager.getLine1Number();
			return number;
		}
		else if (name.equalsIgnoreCase("device_name")) {
			return Build.DEVICE;
		}
		else if (name.equalsIgnoreCase("os_version")) {
			return Build.VERSION.RELEASE;
		}
		
		return null;
	}
	
	public static void exit() {
		RhodesService.getInstance().exitApp();
	}
	
	public static String getTimezoneStr() {
		Calendar cal = Calendar.getInstance();
		TimeZone tz = cal.getTimeZone();
		return tz.getDisplayName();
	}
	
	
}

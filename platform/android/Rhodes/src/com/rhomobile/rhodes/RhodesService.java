package com.rhomobile.rhodes;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.util.Calendar;
import java.util.Enumeration;
import java.util.Locale;
import java.util.Set;
import java.util.TimeZone;
import java.util.UUID;
import java.util.Vector;

import com.rhomobile.rhodes.alert.Alert;
import com.rhomobile.rhodes.event.EventStore;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.geolocation.GeoLocation;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.ui.AboutDialog;
import com.rhomobile.rhodes.ui.LogOptionsDialog;
import com.rhomobile.rhodes.ui.LogViewDialog;
import com.rhomobile.rhodes.uri.ExternalHttpHandler;
import com.rhomobile.rhodes.uri.LocalFileHandler;
import com.rhomobile.rhodes.uri.MailUriHandler;
import com.rhomobile.rhodes.uri.SmsUriHandler;
import com.rhomobile.rhodes.uri.TelUriHandler;
import com.rhomobile.rhodes.uri.UriHandler;
import com.rhomobile.rhodes.uri.VideoUriHandler;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Binder;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.os.PowerManager;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.RemoteViews;

public class RhodesService extends Service {
	
	private static final String TAG = RhodesService.class.getSimpleName();
	
	private static final boolean DEBUG = true;
	
	public static final String INTENT_EXTRA_PREFIX = "com.rhomobile.rhodes.";
	
	public static final String INTENT_SOURCE = INTENT_EXTRA_PREFIX + ".intent.source";
	
	public static int WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
	public static int WINDOW_MASK = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
	public static boolean ANDROID_TITLE = true;
	
	private static final int DOWNLOAD_PACKAGE_ID = 1;
	
	private static final String ACTION_ASK_CANCEL_DOWNLOAD = "com.rhomobile.rhodes.DownloadManager.ACTION_ASK_CANCEL_DOWNLOAD";
	private static final String ACTION_CANCEL_DOWNLOAD = "com.rhomobile.rhodes.DownloadManager.ACTION_CANCEL_DOWNLOAD";
	
	private static RhodesService sInstance;
	
	private final IBinder mBinder = new LocalBinder();
	
	@SuppressWarnings("rawtypes")
	private static final Class[] mStartForegroundSignature = new Class[] {int.class, Notification.class};
	@SuppressWarnings("rawtypes")
	private static final Class[] mStopForegroundSignature = new Class[] {boolean.class};
	@SuppressWarnings("rawtypes")
	private static final Class[] mSetForegroundSignature = new Class[] {boolean.class};
	
	private Method mStartForeground;
	private Method mStopForeground;
	private Method mSetForeground;
	
	private NotificationManager mNM;
	
	//private MainView mMainView;
	
	private static int mScreenWidth;
	private static int mScreenHeight;
	private static int mScreenOrientation;
	
	private static float mScreenPpiX;
	private static float mScreenPpiY;
	
	private static boolean mCameraAvailable;
	
	private static int sActivitiesActive;
	private static boolean sRhodesActivityStarted = false;
	
	synchronized
	static void rhodesActivityStarted(boolean started) { sRhodesActivityStarted = started; }
	
	synchronized
	public static boolean isRhodesActivityStarted() { return sRhodesActivityStarted; }
	
	private static int mGeoLocationInactivityTimeout;

	public static int getGeoLocationInactivityTimeout() {
		return mGeoLocationInactivityTimeout;
	}
	
	private RhoLogConf m_rhoLogConf = new RhoLogConf();
	public RhoLogConf getLogConf() {
		return m_rhoLogConf;
	}
	
	private boolean mNeedGeoLocationRestart = false;
	
	class PowerWakeLock {
	    private PowerManager.WakeLock wakeLockObject = null;
	    private boolean wakeLockEnabled = false;

        synchronized boolean isHeld() {
            return (wakeLockObject != null) && wakeLockObject.isHeld(); 
        }
        synchronized boolean acquire(boolean enable) {
            if (enable) {
                Logger.I(TAG, "Enable WakeLock");
                wakeLockEnabled = true;
            }
            if (wakeLockEnabled) {
                if (wakeLockObject == null) {
                    PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
                    if (pm != null) {
                        Logger.I(TAG, "Acquire WakeLock");
                        wakeLockObject = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, TAG);
                        wakeLockObject.setReferenceCounted(false);
                        wakeLockObject.acquire();
                    }
                    else {
                        Logger.E(TAG, "Can not get PowerManager to acquire WakeLock!!!");
                    }
                    return false;
                }
                //return wakeLockObject.isHeld();
                return true;
            }
            return false;
        }
        synchronized boolean release()
        {
            if (wakeLockObject != null) {
                Logger.I(TAG, "Release WakeLock");
                wakeLockObject.release();
                wakeLockObject = null;
                return true;
            }
            return false;
	    }
	    
	    synchronized boolean reset() {
            if (wakeLockObject != null) {
                Logger.I(TAG, "Reset WakeLock");
                wakeLockObject.release();
                wakeLockObject = null;
                wakeLockEnabled = false;
                return true;
            }
            return false;
	    }
	}
	
	private PowerWakeLock wakeLock = new PowerWakeLock();
	
	private Vector<UriHandler> mUriHandlers = new Vector<UriHandler>();
	
	public boolean handleUrlLoading(String url) {
		Enumeration<UriHandler> e = mUriHandlers.elements();
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
	
	private native void initClassLoader(ClassLoader c);
	
	public native void doSyncAllSources(boolean v);
	public native void doSyncSource(String source);
	
	public native String normalizeUrl(String url);
	
	public native void doRequest(String url);
	
	public static native void loadUrl(String url);
	
	public static native void navigateBack();
	
	private String mRootPath;
	private native void nativeInitPath(String rootPath, String sqliteJournalsPath, String apkPath);
	
	public static native void onScreenOrientationChanged(int width, int height, int angle);
	
	public native void callUiCreatedCallback();
	public native void callUiDestroyedCallback();
	public native void callActivationCallback(boolean active);
	
	public static native String getBuildConfig(String key);
	
	public static native boolean isOnStartPage();
	
	public static RhodesService getInstance() {
		return sInstance;
	}
	
	public static native boolean isTitleEnabled();
	
	
    // TODO: Move these methods to RhodesApplication class
	private void initRootPath() {
		ApplicationInfo appInfo = getAppInfo();
		String dataDir = appInfo.dataDir;
		
		mRootPath = dataDir + "/rhodata/";
		Log.d(TAG, "Root path: " + mRootPath);
		
		String sqliteJournalsPath = dataDir + "/sqlite_stmt_journals/";
		Log.d(TAG, "Sqlite journals path: " + sqliteJournalsPath);
		
		File f = new File(mRootPath);
		f.mkdirs();
		f = new File(f, "db/db-files");
		f.mkdirs();
		f = new File(sqliteJournalsPath);
		f.mkdirs();
		f = new File(mRootPath, "tmp");
		f.mkdirs();
		
		String apkPath = appInfo.sourceDir;
		
		nativeInitPath(mRootPath, sqliteJournalsPath, apkPath);
	}
	
	public String getRootPath() {
		return mRootPath;
	}
	
    // TODO: Move these methods to RhodesApplication class
	private ApplicationInfo getAppInfo() {
		Context context = this;
		String pkgName = context.getPackageName();
		try {
			ApplicationInfo info = context.getPackageManager().getApplicationInfo(pkgName, 0);
			return info;
		} catch (NameNotFoundException e) {
			throw new RuntimeException("Internal error: package " + pkgName + " not found: " + e.getMessage());
		}
	}
	
	public class LocalBinder extends Binder {
		RhodesService getService() {
			return RhodesService.this;
		}
	};

	@Override
	public IBinder onBind(Intent intent) {
		if (DEBUG)
			Log.d(TAG, "+++ onBind");
		return mBinder;
	}
	
	@Override
	public void onCreate() {
		
		if (DEBUG)
			Log.d(TAG, "+++ onCreate");
		
		sInstance = this;
		
		Context context = this;
		
		mNM = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
		
		initClassLoader(context.getClassLoader());
		
		try {
			initRootPath();
			RhoFileApi.init();
		} catch (IOException e) {
			Log.e(TAG, e.getMessage());
			exit();
			return;
		}
		
		if (Utils.isAppHashChanged()) {
			try {
				Log.i(TAG, "Application hash was changed");
				
				File libDir = new File(getRootPath(), "lib");
				File testLib = new File(libDir.getPath(), "rhoframework.iseq");
				if(libDir.isDirectory() && testLib.isFile())
				{
				    Log.i(TAG, "Updating from very old rhodes version, clean filesystem.");
				    Utils.deleteChildrenIgnoreFirstLevel(new File(getRootPath(), "apps"), "rhoconfig.txt");
				    Utils.deleteRecursively(libDir);
				}
				
				LocalFileProvider.revokeUriPermissions(context);

				initRootPath();
				RhoFileApi.init();
				RhoFileApi.copy("hash");
			} catch (IOException e) {
				Log.e(TAG, e.getMessage());
				exit();
				return;
			}
		}
		
		RhodesApplication.create();
		
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra != null) {
			// Show splash screen only if we have active activity
			SplashScreen splashScreen = ra.getSplashScreen();
			splashScreen.start();
		}
		
		initForegroundServiceApi();
		
		setFullscreenParameters();
		
		Logger.I("Rhodes", "Loading...");
		
		// Increase WebView rendering priority
		WebView w = new WebView(context);
		WebSettings webSettings = w.getSettings();
		webSettings.setRenderPriority(WebSettings.RenderPriority.HIGH);
		
		// Get screen width/height
		WindowManager wm = (WindowManager)context.getSystemService(Context.WINDOW_SERVICE);
		Display d = wm.getDefaultDisplay();
		mScreenHeight = d.getHeight();
		mScreenWidth = d.getWidth();
		mScreenOrientation = d.getOrientation();
		
		DisplayMetrics metrics = new DisplayMetrics();
		d.getMetrics(metrics);
		mScreenPpiX = metrics.xdpi;
		mScreenPpiY = metrics.ydpi;
		
		// TODO: detect camera availability
		mCameraAvailable = true;
		
		mGeoLocationInactivityTimeout = RhoConf.getInt("geo_location_inactivity_timeout");
		if (mGeoLocationInactivityTimeout == 0)
			mGeoLocationInactivityTimeout = 25*1000; // 25s
		
		// Register custom uri handlers here
		mUriHandlers.addElement(new ExternalHttpHandler(context));
		mUriHandlers.addElement(new LocalFileHandler(context));
		mUriHandlers.addElement(new MailUriHandler(context));
		mUriHandlers.addElement(new TelUriHandler(context));
		mUriHandlers.addElement(new SmsUriHandler(context));
		mUriHandlers.addElement(new VideoUriHandler(context));
		
		try {
			if (Capabilities.PUSH_ENABLED) {
				if (getPushRegistrationId().length() == 0)
					PushService.register();
			}
		} catch (IllegalAccessException e) {
			Log.e(TAG, e.getMessage());
			exit();
			return;
		}
		
		RhodesApplication.start();
		
		if (sActivitiesActive > 0)
			handleAppActivation();
	}
	
	private void setFullscreenParameters() {
		boolean fullScreen = true;
		if (RhoConf.isExist("full_screen"))
			fullScreen = RhoConf.getBool("full_screen");
		if (fullScreen) {
			WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FULLSCREEN;
			WINDOW_MASK = WindowManager.LayoutParams.FLAG_FULLSCREEN;
			RhodesActivity.setFullscreen(1);
		}
		else {
			WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
			WINDOW_MASK = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
			RhodesActivity.setFullscreen(0);
		}
		//Utils.platformLog(TAG, "rhoconfig    full_screen = "+String.valueOf(fullScreen)+" ");
	}
	
	private void initForegroundServiceApi() {
		try {
			mStartForeground = getClass().getMethod("startForeground", mStartForegroundSignature);
			mStopForeground = getClass().getMethod("stopForeground", mStopForegroundSignature);
			mSetForeground = getClass().getMethod("setForeground", mSetForegroundSignature);
		}
		catch (NoSuchMethodException e) {
			mStartForeground = null;
			mStopForeground = null;
			mSetForeground = null;
		}
	}
	
	@Override
	public void onDestroy() {
	
		if(DEBUG)
			Log.d(TAG, "+++ onDestroy");
		sInstance = null;
		RhodesApplication.stop();
	}
	
	@Override
	public void onStart(Intent intent, int startId) {
		if (DEBUG)
			Log.d(TAG, "+++ onStart");
		try {
			handleCommand(intent, startId);
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't handle service command: " + e.getMessage());
		}
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		if (DEBUG)
			Log.d(TAG, "+++ onStartCommand");
		try {
			handleCommand(intent, startId);
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't handle service command: " + e.getMessage());
		}
		return Service.START_STICKY;
	}
	
	private void handleCommand(Intent intent, int startId) {
		String source = intent.getStringExtra(INTENT_SOURCE);
		Logger.D(TAG, "handleCommand: startId=" + startId + ", source=" + source);
		if (source == null)
			throw new IllegalArgumentException("Service command received from empty source");
		
		if (source.equals(BaseActivity.INTENT_SOURCE)) {
			Logger.D(TAG, "New activity was created");
		}
		else if (source.equals(PushReceiver.INTENT_SOURCE)) {
			int type = intent.getIntExtra(PushReceiver.INTENT_TYPE, PushReceiver.INTENT_TYPE_UNKNOWN);
			switch (type) {
			case PushReceiver.INTENT_TYPE_REGISTRATION_ID:
				String id = intent.getStringExtra(PushReceiver.INTENT_REGISTRATION_ID);
				if (id == null)
					throw new IllegalArgumentException("Empty registration id received in service command");
				Logger.I(TAG, "Received PUSH registration id: " + id);
				setPushRegistrationId(id);
				break;
			case PushReceiver.INTENT_TYPE_MESSAGE:
				Bundle extras = intent.getBundleExtra(PushReceiver.INTENT_EXTRAS);
				Logger.D(TAG, "Received PUSH message: " + extras);
				handlePushMessage(extras);
				break;
			default:
				Logger.W(TAG, "Unknown command type received from " + source + ": " + type);
			}
		}
	}
	
	public void startServiceForeground(int id, Notification notification) {
		if (mStartForeground != null) {
			try {
				mStartForeground.invoke(this, new Object[] {Integer.valueOf(id), notification});
			}
			catch (InvocationTargetException e) {
				Log.e(TAG, "Unable to invoke startForeground", e);
			}
			catch (IllegalAccessException e) {
				Log.e(TAG, "Unable to invoke startForeground", e);
			}
			return;
		}
		
		if (mSetForeground != null) {
			try {
				mSetForeground.invoke(this, new Object[] {Boolean.valueOf(true)});
			}
			catch (InvocationTargetException e) {
				Log.e(TAG, "Unable to invoke setForeground", e);
			}
			catch (IllegalAccessException e) {
				Log.e(TAG, "Unable to invoke setForeground", e);
			}
		}
		mNM.notify(id, notification);
	}
	
	public void stopServiceForeground(int id) {
		if (mStopForeground != null) {
			try {
				mStopForeground.invoke(this, new Object[] {Integer.valueOf(id)});
			}
			catch (InvocationTargetException e) {
				Log.e(TAG, "Unable to invoke stopForeground", e);
			}
			catch (IllegalAccessException e) {
				Log.e(TAG, "Unable to invoke stopForeground", e);
			}
			return;
		}
		
		mNM.cancel(id);
		if (mSetForeground != null) {
			try {
				mSetForeground.invoke(this, new Object[] {Boolean.valueOf(false)});
			}
			catch (InvocationTargetException e) {
				Log.e(TAG, "Unable to invoke setForeground", e);
			}
			catch (IllegalAccessException e) {
				Log.e(TAG, "Unable to invoke setForeground", e);
			}
		}
	}
	
	public void setMainView(MainView v) {
		RhodesActivity ra = RhodesActivity.getInstance();
		ra.setMainView(v);
	}
	
	public MainView getMainView() {
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra == null)
			return null;
		return ra.getMainView();
	}

	public static void exit() {
        try {
            RhodesActivity activity = RhodesActivity.getInstance();
            if (activity != null)
                activity.finish();
            
            RhodesService service = RhodesService.getInstance();
            if (service != null)
            {
                service.wakeLock.reset();
                service.stopSelf();
            }
            
            RhodesApplication.stop();
        }
        catch (Exception e) {
            Logger.E(TAG, e.getMessage());
        }
	}
	
	public void rereadScreenProperties() {
		// check for orientarion changed
		// Get screen width/height
		WindowManager wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
		Display d = wm.getDefaultDisplay();
		mScreenHeight = d.getHeight();
		mScreenWidth = d.getWidth();
		int newScreenOrientation = d.getOrientation();
		if (newScreenOrientation != mScreenOrientation) {
			onScreenOrientationChanged(mScreenWidth, mScreenHeight, 90);
			mScreenOrientation = newScreenOrientation; 				
		}
	}
	
	public static void showAboutDialog() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final AboutDialog aboutDialog = new AboutDialog(RhodesActivity.getInstance());
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
				final LogViewDialog logViewDialog = new LogViewDialog(RhodesActivity.getInstance());
				logViewDialog.setTitle("Log View");
				logViewDialog.setCancelable(true);
				logViewDialog.show();
			}
		}, false);
	}
	
	public static void showLogOptions() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final LogOptionsDialog logOptionsDialog = new LogOptionsDialog(RhodesActivity.getInstance());
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
	
	public static boolean pingHost(String host) {
		HttpURLConnection conn = null;
		boolean hostExists = false;
		try {
				URL url = new URL(host);
				HttpURLConnection.setFollowRedirects(false);
				conn = (HttpURLConnection) url.openConnection();

				conn.setRequestMethod("HEAD");
				conn.setAllowUserInteraction( false );
				conn.setDoInput( true );
				conn.setDoOutput( true );
				conn.setUseCaches( false );

				hostExists = (conn.getContentLength() > 0);
				if(hostExists)
					Logger.I(TAG, "PING network SUCCEEDED.");
				else
					Logger.E(TAG, "PING network FAILED.");
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
		finally {            
		    if (conn != null) 
		    {
		    	try
		    	{
		    		conn.disconnect(); 
		    	}
		    	catch(Exception e) 
		    	{
		    		Logger.E(TAG, e);
		    	}
		    }
		}
		
		return hostExists;
	}
	
	private static boolean hasNetwork() {
		if (!Capabilities.NETWORK_STATE_ENABLED) {
			Logger.E(TAG, "HAS_NETWORK: Capability NETWORK_STATE disabled");
			return false;
		}
		
		Context ctx = RhodesService.getContext();
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
		return mScreenWidth;
	}
	
	public static int getScreenHeight() {
		return mScreenHeight;
	}
	
	public static Object getProperty(String name) {
		try {
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
			else if (name.equalsIgnoreCase("screen_orientation")) {
				if (getScreenWidth() <= getScreenHeight()) {
					return "portrait";
				}
				else {
					return "landscape";
				}
			}
			else if (name.equalsIgnoreCase("has_camera"))
				return new Boolean(mCameraAvailable);
			else if (name.equalsIgnoreCase("has_network"))
				return hasNetwork();
			else if (name.equalsIgnoreCase("ppi_x"))
				return new Float(mScreenPpiX);
			else if (name.equalsIgnoreCase("ppi_y"))
				return new Float(mScreenPpiY);
			else if (name.equalsIgnoreCase("phone_number")) {
				Context context = RhodesService.getContext();
				TelephonyManager manager = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
				String number = manager.getLine1Number();
				return number;
			}
			else if (name.equalsIgnoreCase("device_name")) {
				return Build.DEVICE;
			}
			else if (name.equalsIgnoreCase("is_emulator")) {
			    String strDevice = Build.DEVICE;
				return new Boolean(strDevice != null && strDevice.equalsIgnoreCase("generic"));
			}
			else if (name.equalsIgnoreCase("os_version")) {
				return Build.VERSION.RELEASE;
			}
			else if (name.equalsIgnoreCase("has_calendar")) {
				return new Boolean(EventStore.hasCalendar());
			}
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't get property \"" + name + "\": " + e);
		}
		
		return null;
	}
	
	public static String getTimezoneStr() {
		Calendar cal = Calendar.getInstance();
		TimeZone tz = cal.getTimeZone();
		return tz.getDisplayName();
	}
	
	public static void runApplication(String appName, Object params) {
		try {
			Context ctx = RhodesService.getContext();
			PackageManager mgr = ctx.getPackageManager();
			PackageInfo info = mgr.getPackageInfo(appName, PackageManager.GET_ACTIVITIES);
			if (info.activities.length == 0) {
				Logger.E(TAG, "No activities found for application " + appName);
				return;
			}
			ActivityInfo ainfo = info.activities[0];
			String className = ainfo.name;
			if (className.startsWith("."))
				className = ainfo.packageName + className;

			Intent intent = new Intent();
			intent.setClassName(appName, className);
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			if (params != null) {
				Bundle startParams = new Bundle();
				if (params instanceof String) {
					startParams.putString(RhodesActivity.RHO_START_PARAMS_KEY, (String)params);
				}
				/*
				else if (params instanceof List<?>) {
					for (Object obj : (List<?>)params) {
						startParams.putInt(obj.toString(), 1);
					}
				}
				else if (params instanceof Map<?,?>) {
					Map<?,?> mp = (Map<?,?>)params;
					for (Iterator<?> it = mp.keySet().iterator(); it.hasNext();) {
						Object key = it.next();
						Object value = mp.get(key);
						startParams.putString(key.toString(), value == null ? null : value.toString());
					}
				}
				*/
				else
					throw new IllegalArgumentException("Unknown type of incoming parameter");

				intent.putExtras(startParams);
			}
			ctx.startActivity(intent);
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't run application " + appName + ": " + e.getMessage());
		}
	}
	
	public static boolean isAppInstalled(String appName) {
		try {
			RhodesService.getContext().getPackageManager().getPackageInfo(appName, 0);
			return true;
		}
		catch (NameNotFoundException ne) {
			return false;
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't check is app " + appName + " installed: " + e.getMessage());
			return false;
		}
	}
	
	private void updateDownloadNotification(String url, int totalBytes, int currentBytes) {
		Context context = RhodesActivity.getContext();
		
		Notification n = new Notification();
		n.icon = android.R.drawable.stat_sys_download;
		n.flags |= Notification.FLAG_ONGOING_EVENT;
		
		RemoteViews expandedView = new RemoteViews(context.getPackageName(),
				AndroidR.layout.status_bar_ongoing_event_progress_bar);
		
		StringBuilder newUrl = new StringBuilder();
		if (url.length() < 17)
			newUrl.append(url);
		else {
			newUrl.append(url.substring(0, 7));
			newUrl.append("...");
			newUrl.append(url.substring(url.length() - 7, url.length()));
		}
		expandedView.setTextViewText(AndroidR.id.title, newUrl.toString());
		
		StringBuffer downloadingText = new StringBuffer();
		if (totalBytes > 0) {
			long progress = currentBytes*100/totalBytes;
			downloadingText.append(progress);
			downloadingText.append('%');
		}
		expandedView.setTextViewText(AndroidR.id.progress_text, downloadingText.toString());
		expandedView.setProgressBar(AndroidR.id.progress_bar,
				totalBytes < 0 ? 100 : totalBytes,
				currentBytes,
				totalBytes < 0);
		n.contentView = expandedView;
		
		Intent intent = new Intent(ACTION_ASK_CANCEL_DOWNLOAD);
		n.contentIntent = PendingIntent.getBroadcast(context, 0, intent, 0);
		intent = new Intent(ACTION_CANCEL_DOWNLOAD);
		n.deleteIntent = PendingIntent.getBroadcast(context, 0, intent, 0);

		mNM.notify(DOWNLOAD_PACKAGE_ID, n);
	}
	
	private File downloadPackage(String url) throws IOException {
		final Context ctx = RhodesActivity.getContext();
		
		final Thread thisThread = Thread.currentThread();
		
		final Runnable cancelAction = new Runnable() {
			public void run() {
				thisThread.interrupt();
			}
		};
		
		BroadcastReceiver downloadReceiver = new BroadcastReceiver() {
			@Override
			public void onReceive(Context context, Intent intent) {
				String action = intent.getAction();
				if (action.equals(ACTION_ASK_CANCEL_DOWNLOAD)) {
					AlertDialog.Builder builder = new AlertDialog.Builder(ctx);
					builder.setMessage("Cancel download?");
					AlertDialog dialog = builder.create();
					dialog.setButton(AlertDialog.BUTTON_POSITIVE, ctx.getText(android.R.string.yes),
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int which) {
									cancelAction.run();
								}
					});
					dialog.setButton(AlertDialog.BUTTON_NEGATIVE, ctx.getText(android.R.string.no),
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int which) {
									// Nothing
								}
							});
					dialog.show();
				}
				else if (action.equals(ACTION_CANCEL_DOWNLOAD)) {
					cancelAction.run();
				}
			}
		};
		IntentFilter filter = new IntentFilter();
		filter.addAction(ACTION_ASK_CANCEL_DOWNLOAD);
		filter.addAction(ACTION_CANCEL_DOWNLOAD);
		ctx.registerReceiver(downloadReceiver, filter);
		
		File tmpFile = null;
		InputStream is = null;
		OutputStream os = null;
		try {
			updateDownloadNotification(url, -1, 0);
			
			/*
			List<File> folders = new ArrayList<File>();
			folders.add(Environment.getDownloadCacheDirectory());
			folders.add(Environment.getDataDirectory());
			folders.add(ctx.getCacheDir());
			folders.add(ctx.getFilesDir());
			try {
				folders.add(new File(ctx.getPackageManager().getApplicationInfo(ctx.getPackageName(), 0).dataDir));
			} catch (NameNotFoundException e1) {
				// Ignore
			}
			folders.add(Environment.getExternalStorageDirectory());
			
			for (File folder : folders) {
				File tmpRootFolder = new File(folder, "rhodownload");
				File tmpFolder = new File(tmpRootFolder, ctx.getPackageName());
				if (tmpFolder.exists())
					deleteFilesInFolder(tmpFolder.getAbsolutePath());
				else
					tmpFolder.mkdirs();
				
				File of = new File(tmpFolder, UUID.randomUUID().toString() + ".apk");
				Logger.D(TAG, "Check path " + of.getAbsolutePath() + "...");
				try {
					os = new FileOutputStream(of);
				}
				catch (FileNotFoundException e) {
					Logger.D(TAG, "Can't open file " + of.getAbsolutePath() + ", check next path");
					continue;
				}
				Logger.D(TAG, "File " + of.getAbsolutePath() + " succesfully opened for write, start download app");
				
				tmpFile = of;
				break;
			}
			*/
			
			tmpFile = ctx.getFileStreamPath(UUID.randomUUID().toString() + ".apk");
			os = ctx.openFileOutput(tmpFile.getName(), Context.MODE_WORLD_READABLE);
			
			Logger.D(TAG, "Download " + url + " to " + tmpFile.getAbsolutePath() + "...");
			
			URL u = new URL(url);
			URLConnection conn = u.openConnection();
			int totalBytes = -1;
			if (conn instanceof HttpURLConnection) {
				HttpURLConnection httpConn = (HttpURLConnection)conn;
				totalBytes = httpConn.getContentLength();
			}
			is = conn.getInputStream();
			
			int downloaded = 0;
			updateDownloadNotification(url, totalBytes, downloaded);
			
			long prevProgress = 0;
			byte[] buf = new byte[65536];
			for (;;) {
				if (thisThread.isInterrupted()) {
					tmpFile.delete();
					Logger.D(TAG, "Download of " + url + " was canceled");
					return null;
				}
				int nread = is.read(buf);
				if (nread == -1)
					break;
				
				//Logger.D(TAG, "Downloading " + url + ": got " + nread + " bytes...");
				os.write(buf, 0, nread);
				
				downloaded += nread;
				if (totalBytes > 0) {
					// Update progress view only if current progress is greater than
					// previous by more than 10%. Otherwise, if update it very frequently,
					// user will no have chance to click on notification view and cancel if need
					long progress = downloaded*10/totalBytes;
					if (progress > prevProgress) {
						updateDownloadNotification(url, totalBytes, downloaded);
						prevProgress = progress;
					}
				}
			}
			
			Logger.D(TAG, "File stored to " + tmpFile.getAbsolutePath());
			
			return tmpFile;
		}
		catch (IOException e) {
			if (tmpFile != null)
				tmpFile.delete();
			throw e;
		}
		finally {
			try {
				if (is != null)
					is.close();
			} catch (IOException e) {}
			try {
				if (os != null)
					os.close();
			} catch (IOException e) {}
			
			mNM.cancel(DOWNLOAD_PACKAGE_ID);
			ctx.unregisterReceiver(downloadReceiver);
		}
	}
	
	public static void installApplication(final String url) {
		Thread bgThread = new Thread(new Runnable() {
			public void run() {
				try {
					final RhodesService r = RhodesService.getInstance();
					final File tmpFile = r.downloadPackage(url);
					if (tmpFile != null) {
						PerformOnUiThread.exec(new Runnable() {
							public void run() {
								try {
									Logger.D(TAG, "Install package " + tmpFile.getAbsolutePath());
									Uri uri = Uri.fromFile(tmpFile);
									Intent intent = new Intent(Intent.ACTION_VIEW);
									intent.setDataAndType(uri, "application/vnd.android.package-archive");
									r.startActivity(intent);
								}
								catch (Exception e) {
									Log.e(TAG, "Can't install file from " + tmpFile.getAbsolutePath(), e);
									Logger.E(TAG, "Can't install file from " + tmpFile.getAbsolutePath() + ": " + e.getMessage());
								}
							}
						}, false);
					}
				}
				catch (IOException e) {
					Log.e(TAG, "Can't download package from " + url, e);
					Logger.E(TAG, "Can't download package from " + url + ": " + e.getMessage());
				}
			}
		});
		bgThread.setPriority(Thread.MIN_PRIORITY);
		bgThread.start();
	}
	
	public static void uninstallApplication(String appName) {
		try {
			Uri packageUri = Uri.parse("package:" + appName);
			Intent intent = new Intent(Intent.ACTION_DELETE, packageUri);
			RhodesService.getContext().startActivity(intent);
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't uninstall application " + appName + ": " + e.getMessage());
		}
	}

    /** Opens remote or local URL*/
    public static void openExternalUrl(String url)
    {
        try
        {
            if(url.charAt(0) == '/')
                url = "file://" + RhoFileApi.absolutePath(url);

            //FIXME: Use common URI handling
            Context ctx = RhodesService.getContext();
            LocalFileHandler fileHandler = new LocalFileHandler(ctx);
            if(!fileHandler.handle(url))
            {
                Logger.D(TAG, "Handling URI: " + url);

                Intent intent = Intent.parseUri(url, 0);
                ctx.startActivity(Intent.createChooser(intent, "Open in..."));
            }
        }
        catch (Exception e) {
            Logger.E(TAG, "Can't open url :'" + url + "': " + e.getMessage());
        }
    }

	private native void setPushRegistrationId(String id);
	private native String getPushRegistrationId(); 
	
	private native boolean callPushCallback(String data);
	
	private void handlePushMessage(Bundle extras) {
		Logger.D(TAG, "Receive PUSH message");
		
		if (extras == null) {
			Logger.W(TAG, "Empty PUSH message received");
			return;
		}
		
		StringBuilder builder = new StringBuilder();
		
		Set<String> keys = extras.keySet();
		// Remove system related keys
		keys.remove("collapse_key");
		keys.remove("from");
		
		for (String key : keys) {
			Logger.D(TAG, "PUSH item: " + key);
			Object value = extras.get(key);
			if (builder.length() > 0)
				builder.append("&");
			builder.append(key);
			builder.append("=");
			if (value != null)
				builder.append(value.toString());
		}
		
		String data = builder.toString();
		Logger.D(TAG, "Received PUSH message: " + data);
		if (callPushCallback(data))
			return;
		
		String alert = extras.getString("alert");
		if (alert != null) {
			Logger.D(TAG, "PUSH: Alert: " + alert);
			Alert.showPopup(alert);
		}
		String sound = extras.getString("sound");
		if (sound != null) {
			Logger.D(TAG, "PUSH: Sound file name: " + sound);
			Alert.playFile("/public/alerts/" + sound, null);
		}
		String vibrate = extras.getString("vibrate");
		if (vibrate != null) {
			Logger.D(TAG, "PUSH: Vibrate: " + vibrate);
			int duration;
			try {
				duration = Integer.parseInt(vibrate);
			}
			catch (NumberFormatException e) {
				duration = 5;
			}
			Logger.D(TAG, "Vibrate " + duration + " seconds");
			Alert.vibrate(duration);
		}
		
		String syncSources = extras.getString("do_sync");
		if (syncSources != null) {
			Logger.D(TAG, "PUSH: Sync:");
			boolean syncAll = false;
			for (String source : syncSources.split(",")) {
				Logger.D(TAG, "url = " + source);
				if (source.equalsIgnoreCase("all"))
					syncAll = true;
				else {
					doSyncSource(source.trim());
				}
			}
			
			if (syncAll)
				doSyncAllSources(true);
		}
	}
	
	private void restartGeoLocationIfNeeded() {
		if (mNeedGeoLocationRestart) {
			GeoLocation.isKnownPosition();
			mNeedGeoLocationRestart = false;
		}
	}
	
	private void stopGeoLocation() {
		mNeedGeoLocationRestart = GeoLocation.isAvailable();
		GeoLocation.stop();
	}
	
	private void restoreWakeLockIfNeeded() {
		 wakeLock.acquire(false);
	}
	
	private void stopWakeLock() {
		Logger.I(TAG, "activityStopped() temporary release wakeLock object");
		wakeLock.release();
	}
	
	public static int rho_sys_set_sleeping(int enable) {
		Logger.I(TAG, "rho_sys_set_sleeping("+enable+")");
		RhodesService rs = RhodesService.getInstance();
        int wasEnabled = rs.wakeLock.isHeld() ? 1 : 0;
		if(rs != null)
		{
		
	        if (enable != 0) {
	            // disable lock device
				wasEnabled = rs.wakeLock.reset() ? 1 : 0;
			}
	        else {
	            // lock device from sleep
			    PerformOnUiThread.exec(new Runnable() {
			        public void run() {
			            RhodesService rs = RhodesService.getInstance();
			            if(rs != null) rs.wakeLock.acquire(true);
			            else Logger.E(TAG, "rho_sys_set_sleeping() - No RhodesService has initialized !!!"); }
			    }, false);
			}
		}
		return wasEnabled;
	}
	
	private void handleAppActivation() {
		if (DEBUG)
			Log.d(TAG, "handle app activation");
		restartGeoLocationIfNeeded();
		restoreWakeLockIfNeeded();
		callActivationCallback(true);
	}
	
	private void handleAppDeactivation() {
		if (DEBUG)
			Log.d(TAG, "handle app deactivation");
		stopWakeLock();
		stopGeoLocation();
		callActivationCallback(false);
	}
	
	public static void activityStarted() {
		if (DEBUG)
			Log.d(TAG, "activityStarted (1): sActivitiesActive=" + sActivitiesActive);
		if (sActivitiesActive == 0) {
			RhodesService r = RhodesService.getInstance();
			if (DEBUG)
				Log.d(TAG, "first activity started; r=" + r);
			if (r != null)
				r.handleAppActivation();
		}
		++sActivitiesActive;
		if (DEBUG)
			Log.d(TAG, "activityStarted (2): sActivitiesActive=" + sActivitiesActive);
	}
	
	public static void activityStopped() {
		if (DEBUG)
			Log.d(TAG, "activityStopped (1): sActivitiesActive=" + sActivitiesActive);
		--sActivitiesActive;
		if (sActivitiesActive == 0) {
			RhodesService r = RhodesService.getInstance();
			if (DEBUG)
				Log.d(TAG, "last activity stopped; r=" + r);
			if (r != null)
				r.handleAppDeactivation();
		}
		if (DEBUG)
			Log.d(TAG, "activityStopped (2): sActivitiesActive=" + sActivitiesActive);
	}
	
	@Override
	public void startActivity(Intent intent) {
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra == null)
			throw new IllegalStateException("Trying to start activity, but main activity is empty (we are in background, no UI active)");
		ra.startActivity(intent);
	}
	
	public static Context getContext() {
		RhodesService r = RhodesService.getInstance();
		if (r == null)
			throw new IllegalStateException("No rhodes service instance at this moment");
		return r;
	}
	
	public static boolean isJQTouch_mode() {
		return RhoConf.getBool("jqtouch_mode");
	}
	
}

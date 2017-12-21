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

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.BufferedReader;
import java.io.FileReader;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.HttpURLConnection;
import java.net.URISyntaxException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Calendar;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TimeZone;
import java.util.UUID;
import java.util.Vector;

import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import com.rhomobile.rhodes.event.EventStore;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.geolocation.GeoLocation;
import com.rhomobile.rhodes.mainview.MainView;
import com.rhomobile.rhodes.mainview.SplashScreen;
import com.rhomobile.rhodes.osfunctionality.AndroidFunctionalityManager;
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
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.JSONGenerator;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.PhoneId;
import com.rhomobile.rhodes.util.Utils;
//import com.rhomobile.rhodes.camera.Camera;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ActivityNotFoundException;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.pm.FeatureInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.PermissionInfo;
import android.content.pm.ResolveInfo;
import android.content.res.Configuration;
import android.hardware.Camera;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.Process;
import android.text.InputType;
import android.text.TextUtils;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.util.StringBuilderPrinter;
import android.view.WindowManager;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.RemoteViews;
import android.widget.EditText;
import android.os.Environment;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager.NameNotFoundException;


import java.util.Enumeration;
import java.net.NetworkInterface;
import java.net.InetAddress;
import java.net.Inet4Address;
import java.net.SocketException;

public class RhodesService extends Service {
	
	private static final String TAG = RhodesService.class.getSimpleName();
	
	private static final boolean DEBUG = false;
	
	public static final String INTENT_EXTRA_PREFIX = "com.rhomobile.rhodes";
	
	public static final String INTENT_SOURCE = INTENT_EXTRA_PREFIX + ".intent_source";
	
	public static final String INTENT_EXTRA_MESSAGE = INTENT_EXTRA_PREFIX + ".message";
 	
	public static int WINDOW_FLAGS = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
	public static int WINDOW_MASK = WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN;
	public static boolean ANDROID_TITLE = true;
	
	private static final int DOWNLOAD_PACKAGE_ID = 1;
	
	private static final String ACTION_ASK_CANCEL_DOWNLOAD = "com.rhomobile.rhodes.DownloadManager.ACTION_ASK_CANCEL_DOWNLOAD";
	private static final String ACTION_CANCEL_DOWNLOAD = "com.rhomobile.rhodes.DownloadManager.ACTION_CANCEL_DOWNLOAD";

    private static final String NOTIFICATION_NONE = "none";
    private static final String NOTIFICATION_BACKGROUND = "background";
    private static final String NOTIFICATION_ALWAYS = "always";

	private static RhodesService sInstance = null;
	
	private final IBinder mBinder = new LocalBinder();
	
	private BroadcastReceiver mConnectionChangeReceiver;
	
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
	
	private boolean mNeedGeoLocationRestart = false;
	
	public static String               m_Text                      = "";
	private static String				ExitPasswordEnabled         = "";
	private static String				ExitPasswordValue           = "";
	
	public static  void setExitPasswordEnabled(String exitPasswordEnabled)
	{ 
	  ExitPasswordEnabled=exitPasswordEnabled;
	}
	
	public static  void setExitPasswordValue(String exitPasswordValue)
	{
		ExitPasswordValue=exitPasswordValue;
	}
	
	public static  String getExitPasswordEnabled()
	{
		return ExitPasswordEnabled;
	}
	
	public static  String getExitPasswordValue()
	{
		return ExitPasswordValue;
	}
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
	
	Handler mHandler = null;
	
	public native void doSyncAllSources(boolean v);
	public native void doSyncSource(String source);
	
	public native String normalizeUrl(String url);
	
	public static native void doRequest(String url);
	public static native void doRequestAsync(String url);
	public static native void doRequestEx(String url, String body, String data, boolean waitForResponse);
    public static native void doRequestJson(String url, String body, String data, boolean waitForResponse);
    
    public static native void loadUrl(String url);
    public static native String currentLocation(int tab);
	
	public static native void navigateBack();
	
	public static native void onScreenOrientationChanged(int width, int height, int angle);
	
	public static native void callUiCreatedCallback();
	public static native void callUiDestroyedCallback();
	public static native void callActivationCallback(boolean active);
	
	public static native String getBuildConfig(String key);
	
	public static native String getInvalidSecurityTokenMessage();
	
	public static native void resetHttpLogging(String http_log_url);
	public static native void resetFileLogging(String log_path);
		
	public native void notifyNetworkStatusChanged( int status );
	
	
	public static RhodesService getInstance() {
		return sInstance;
	}
	
	public static native boolean isTitleEnabled();
	
	private static final String CONF_PHONE_ID = "phone_id";
	private PhoneId getPhoneId() {
	    String strPhoneId = RhoConf.getString(CONF_PHONE_ID);
	    PhoneId phoneId = PhoneId.getId(this, strPhoneId);
	    if (strPhoneId == null || strPhoneId.length() == 0)
	        RhoConf.setString(CONF_PHONE_ID, phoneId.toString());

	    return phoneId;
	}
		public static void setStartPath(String startPath)
	{
	         String startPathName = "start_path";
		 String externalSharedPath = Environment.getExternalStorageDirectory().getAbsolutePath() ;
		 StringBuffer str = new StringBuffer(startPath);
		 if(startPath.contains("file") && !startPath.contains("sdcard") && !startPath.contains("internal"))
		 {
		 	int index=7;
		 str = str.insert(7,externalSharedPath);
		 }
		 if(startPath != null)
          		RhoConf.setString(startPathName, str.toString());
	}
	
	public class LocalBinder extends Binder {
		RhodesService getService() {
			return RhodesService.this;
		}
	};

	@Override
	public IBinder onBind(Intent intent) {
		Logger.D(TAG, "onBind");
		return mBinder;
	}

	@Override
	public void onCreate() {
		Logger.D(TAG, "onCreate");

		sInstance = this;

		Context context = this;

		mNM = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);

        LocalFileProvider.revokeUriPermissions(this);

        Logger.I("Rhodes", "Loading...");
        RhodesApplication.create();

        // Increase WebView rendering priority
        WebView w = new WebView(context);
        WebSettings webSettings = w.getSettings();
        webSettings.setRenderPriority(WebSettings.RenderPriority.HIGH);

        try {
            // Show splash screen only if we have active activity
            SplashScreen splashScreen = RhodesActivity.safeGetInstance().getSplashScreen();
            if (splashScreen != null) {
                splashScreen.start();
            }
        } catch (NullPointerException ex) {
            
        }

		initForegroundServiceApi();

		// Register custom uri handlers here
		mUriHandlers.addElement(new ExternalHttpHandler(context));
		mUriHandlers.addElement(new LocalFileHandler(context));
		mUriHandlers.addElement(new MailUriHandler(context));
		mUriHandlers.addElement(new TelUriHandler(context));
		mUriHandlers.addElement(new SmsUriHandler(context));
		mUriHandlers.addElement(new VideoUriHandler(context));

        mConnectionChangeReceiver = new ConnectionChangeReceiver();
        IntentFilter filter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);
        registerReceiver(mConnectionChangeReceiver,filter);

		RhodesApplication.start();

		if (BaseActivity.getActivitiesCount() > 0)
			handleAppActivation();
	}

	public static void handleAppStarted()
	{
	    RhodesApplication.handleAppStarted();
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
		Log.d(TAG, "onStart");
		try {
			handleCommand(intent, startId);
		}
		catch (Exception e) {
			Logger.E(TAG, "Can't handle service command");
			Logger.E(TAG, e);
		}
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Logger.D(TAG, "onStartCommand");
		try {
			handleCommand(intent, startId);
		}
		catch (Exception e) {
            Logger.E(TAG, "Can't handle service command");
            Logger.E(TAG, e);
		}
		// From start_sticky yo start_not_sticky is done because we dont want OS to start service incase of low memory and recovery. Anyways it is started
		//by the BaseActivity so to maintain the same state it is better if OS does NOT restart.SR EMBPD00183300
		return Service.START_NOT_STICKY;
	}
	
	private void handleCommand(Intent intent, int startId) {
		if (intent == null) {
			return;
		}
		String source = intent.getStringExtra(INTENT_SOURCE);
		Set<String> categories = intent.getCategories();
		Logger.I(TAG, "handleCommand: startId=" + startId + ", source=" + source);

		if (BaseActivity.INTENT_SOURCE.equals(source)) {
			Logger.D(TAG, "New activity was created");
		}
		else if (PushContract.INTENT_SOURCE.equals(source)) {
			int type = intent.getIntExtra(PushContract.INTENT_TYPE, PushContract.INTENT_TYPE_UNKNOWN);
			switch (type) {
			case PushContract.INTENT_TYPE_REGISTRATION_ID: 
			{
				String id = intent.getStringExtra(PushContract.INTENT_REGISTRATION_ID);
                String pushType = intent.getStringExtra(PushContract.INTENT_PUSH_CLIENT);
				if (id == null)
					throw new IllegalArgumentException("Empty registration id received in service command");
				Logger.I(TAG, "Received PUSH registration id: " + id);
				setPushRegistrationId(pushType, id);
			}
				break;
            case PushContract.INTENT_TYPE_MESSAGE:
                if(intent.hasExtra(PushContract.INTENT_MESSAGE_EXTRAS)) {
                    final String pushType = intent.getStringExtra(PushContract.INTENT_PUSH_CLIENT);
                    final Bundle extras = intent.getBundleExtra(PushContract.INTENT_MESSAGE_EXTRAS);
                    Logger.D(TAG, "Received PUSH message: " + extras);
                    RhodesApplication.runWhen(
                        RhodesApplication.AppState.AppStarted,
                        new RhodesApplication.StateHandler(true) {
                            @Override
                            public void run()
                            {
                                handlePushMessage(pushType, extras);
                            }
                        });
                    break;
                }
                else if (intent.hasExtra(PushContract.INTENT_MESSAGE_JSON)){
                    final String pushType = intent.getStringExtra(PushContract.INTENT_PUSH_CLIENT);
                    final String json = intent.getStringExtra(PushContract.INTENT_MESSAGE_JSON);
                    if (json != null) {
                        Logger.D(TAG, "Received PUSH message (JSON): " + json);
                        RhodesApplication.runWhen(
                            RhodesApplication.AppState.AppStarted,
                            new RhodesApplication.StateHandler(true) {
                                @Override
                                public void run()
                                {
                                    handlePushMessage(pushType, json);
                                }
                            });
                    }
                    break;
                }
            default:
                Logger.W(TAG, "Unknown command type received from " + source + ": " + type);
            }
        } else {
            RhoExtManager.getImplementationInstance().onNewIntent(this, intent);
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
	
	public void setMainView(MainView v) throws NullPointerException {
		RhodesActivity.safeGetInstance().setMainView(v);
	}
	
	public MainView getMainView() {
		RhodesActivity ra = RhodesActivity.getInstance();
		if (ra == null)
			return null;
		return ra.getMainView();
	}

	public static void kill() {
		Logger.I(TAG, "kkkill !!!");
		Process.killProcess(Process.myPid());
	}
	private static void displayAlertForPassword()
	{
		PerformOnUiThread.exec(new Runnable(){
			
			@Override
			public void run() {
				PasswordDialog.createpopup();						
				
				
			}
		});
	}
	public static void PerformRealExit()
	{
		  PerformOnUiThread.exec(new Runnable() {
		        @Override
		        public void run() {
	                Logger.I(TAG, "Exit application");
	                try {
	                    // Do this fake state change in order to make processing before server is stopped
	                    RhodesApplication.stateChanged(RhodesApplication.UiState.MainActivityPaused);
	        
	                    RhodesService service = RhodesService.getInstance();
	                    if (service != null)
	                    {
	                        Logger.I(TAG, "stop RhodesService");
	                        service.wakeLock.reset();
	                        service.stopSelf();
	                    }
	                    
	                    Logger.I(TAG, "stop RhodesApplication");
	                    RhodesApplication.stop();
	                }
	                catch (Exception e) {
	                    Logger.E(TAG, e);
	                }
		        }
		    });
	}
	public static void exit() {
		if(getExitPasswordEnabled().equals("1"))
		{      
	                if(getExitPasswordValue().equals(""))
                 	PerformRealExit();		
			else
			displayAlertForPassword();
		}
		else
			PerformRealExit();
	}
	
	public static void showAboutDialog() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final AboutDialog aboutDialog = new AboutDialog(ContextFactory.getUiContext());
				aboutDialog.setTitle("About");
				aboutDialog.setCanceledOnTouchOutside(true);
				aboutDialog.setCancelable(true);
				aboutDialog.show();
			}
		});
	}
	
	public static void showLogView() {
	    getInstance().startActivity(new Intent().setClass(getContext(), LogViewDialog.class));
	}
	
	public static void showLogOptions() {
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				final LogOptionsDialog logOptionsDialog = new LogOptionsDialog(ContextFactory.getUiContext());
				logOptionsDialog.setTitle("Logging Options");
				logOptionsDialog.setCancelable(true);
				logOptionsDialog.show();
			}
		});
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
				conn.setConnectTimeout( 10000 );
				conn.setReadTimeout( 10000 );

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
	
	private static boolean hasNetworkEx( boolean checkCell, boolean checkWifi, boolean checkEthernet, boolean checkWimax, boolean checkBluetooth, boolean checkAny) {
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

		//{
		//	Utils.platformLog("NETWORK", "$$$$$$$$$$$$$$$$$$$   Networks ; $$$$$$$$$$$$$$$$$$$$$$");
		//	for (int i = 0, lim = info.length; i < lim; ++i) 
		//	{
		//		int type = info[i].getType();
		//		String name = info[i].getTypeName();
		//		boolean is_connected = info[i].getState() == NetworkInfo.State.CONNECTED;
		//		Utils.platformLog("NETWORK", "        - Name ["+name+"],  type ["+String.valueOf(type)+"], connected ["+String.valueOf(is_connected)+"]");
		//	}
		//}
		
		for (int i = 0, lim = info.length; i < lim; ++i) 
		{
			boolean is_connected = info[i].getState() == NetworkInfo.State.CONNECTED;
			int type = info[i].getType();
			if (is_connected) {
				if ((type == ConnectivityManager.TYPE_MOBILE) && (checkCell)) return true;
				if ((type == ConnectivityManager.TYPE_WIFI) && (checkWifi)) return true;
				if ((type == 6) && (checkWimax)) return true;
				if ((type == 9) && (checkEthernet)) return true;
				if ((type == 7) && (checkBluetooth)) return true;
				if (checkAny) return true; 
			}
		}

    	Logger.I(TAG, "HAS_NETWORK: all networks are disconnected");
		
		return false;
	}
	
	private static boolean hasWiFiNetwork() {
		return hasNetworkEx(false, true, true, true, false, false);
	}
	
	private static boolean hasCellNetwork() {
		return hasNetworkEx(true, false, false, false, false, false);
	}
	
	private static boolean hasNetwork() {
		return hasNetworkEx(true, true, true, true, false, true);
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
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getWidth();
        else
            return 0;
    }
	
    public static int getScreenHeight() {
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getHeight();
        else
            return 0;
    }
    
    public static int getRealScreenWidth() {
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getRealWidth();
        else
            return 0;
    }
	
    public static int getRealScreenHeight() {
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getRealHeight();
        else
            return 0;
    }

    public static float getScreenPpiX() {
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getPpiX();
        else
            return 0;
    }
    
    public static float getScreenPpiY() {
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getPpiY();
        else
            return 0;
    }

    public static int getScreenOrientation() {
        if (BaseActivity.getScreenProperties() != null)
            return BaseActivity.getScreenProperties().getOrientation();
        else
            return Configuration.ORIENTATION_UNDEFINED;
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
				return Integer.valueOf(getScreenWidth());
			else if (name.equalsIgnoreCase("screen_height"))
				return Integer.valueOf(getScreenHeight());
			else if (name.equalsIgnoreCase("real_screen_width"))
				return Integer.valueOf(getRealScreenWidth());
			else if (name.equalsIgnoreCase("real_screen_height"))
				return Integer.valueOf(getRealScreenHeight());
			else if (name.equalsIgnoreCase("screen_orientation")) {
				int orientation = getScreenOrientation();
				if ((orientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE)
						|| (orientation == ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE))
					return "landscape";
				else
					return "portrait";
			} else if (name.equalsIgnoreCase("has_network"))
				return Boolean.valueOf(hasNetwork());
			else if (name.equalsIgnoreCase("has_wifi_network"))
				return Boolean.valueOf(hasWiFiNetwork());
			else if (name.equalsIgnoreCase("has_cell_network"))
				return Boolean.valueOf(hasCellNetwork());
			else if (name.equalsIgnoreCase("ppi_x"))
				return Float.valueOf(getScreenPpiX());
			else if (name.equalsIgnoreCase("ppi_y"))
				return Float.valueOf(getScreenPpiY());
			else if (name.equalsIgnoreCase("phone_number")) {
				Context context = ContextFactory.getContext();
				String number = "";
				if (context != null) {
					TelephonyManager manager = (TelephonyManager) context
							.getSystemService(Context.TELEPHONY_SERVICE);
					number = manager.getLine1Number();
					Logger.I(TAG, "Phone number: " + number);
				}
				return number;
			} else if (name.equalsIgnoreCase("device_owner_name")) {
				return AndroidFunctionalityManager.getAndroidFunctionality()
						.AccessOwnerInfo_getUsername(getContext());
			} else if (name.equalsIgnoreCase("device_owner_email")) {
				return AndroidFunctionalityManager.getAndroidFunctionality()
						.AccessOwnerInfo_getEmail(getContext());
			} else if (name.equalsIgnoreCase("device_name")) {
				return Build.MANUFACTURER + " " + Build.DEVICE;
			} else if (name.equalsIgnoreCase("is_emulator")) {
				String strDevice = Build.DEVICE;
				return Boolean.valueOf(strDevice != null
						&& strDevice.equalsIgnoreCase("generic"));
			} else if (name.equalsIgnoreCase("os_version")) {
				return Build.VERSION.RELEASE;
			} else if (name.equalsIgnoreCase("has_calendar")) {
				return Boolean.valueOf(EventStore.hasCalendar());
			} else if (name.equalsIgnoreCase("phone_id")) {
				RhodesService service = RhodesService.getInstance();
				if (service != null) {
					PhoneId phoneId = service.getPhoneId();
					return phoneId.toString();
				} else {
					return "";
				}
			} else if (name.equalsIgnoreCase("webview_framework")) {
				return RhodesActivity.safeGetInstance().getMainView()
						.getWebView(-1).getEngineId();
			} else if (name.equalsIgnoreCase("is_symbol_device")) {
				return isSymbolDevice();
			} else if (name.equalsIgnoreCase("oem_info")) {
				return Build.PRODUCT;
			} else if (name.equalsIgnoreCase("uuid")) {
				return fetchUUID();
			} else if (name.equalsIgnoreCase("has_camera")) {
				boolean hasCamera = false;
				/*
				 * try { if (Camera.getCameraService() != null) { if
				 * ((Camera.getCameraService().getMainCamera() != null) ||
				 * (Camera.getCameraService().getFrontCamera() != null)) {
				 * hasCamera = true; } } } catch (Throwable e) {
				 * e.printStackTrace(); Logger.E(TAG,
				 * "Exception during detect Camera for has_camera"); }
				 */
				int noofCameras = Camera.getNumberOfCameras();
				hasCamera = noofCameras == 0 ? false : true;
				return hasCamera;// Boolean.TRUE;
			} else {
				return RhoExtManager.getImplementationInstance().getProperty(
						name);
			}
		} catch (Exception e) {
			Logger.E(TAG, "Can't get property \"" + name + "\": " + e);
		}

		return null;
    }
    
    public static Boolean isSymbolDevice() {
        Boolean res = false;
       /* try
        {
            Class<?> commonClass = Class.forName("com.rho.rhoelements.Common");
            Method isEmdkDeviceMethod = commonClass.getDeclaredMethod("isEmdkDevice");
            res = (Boolean)isEmdkDeviceMethod.invoke(null);
        } 
        catch (Throwable e) { }
        return Boolean.valueOf(Capabilities.SYMBOL_ENABLED && res);*/
        
      //   There is a loading issue if app_type=rhodes. SR EMBPD00111897
        
      return isAppInstalled("com.symbol.emdk.proxyframework") ||
             isAppInstalled("com.symbol.emdk.datawedge") ||
            isAppInstalled("com.symbol.datawedge") || symbolSupportedDeviceID();
    }
    
    private static Boolean symbolSupportedDeviceID(){
    	boolean flag = false;
    	if((Build.MODEL.compareToIgnoreCase("MC18N0") == 0) || (Build.MODEL.compareToIgnoreCase("MC32N0") == 0)
    			|| (Build.MODEL.compareToIgnoreCase("MC40N0") == 0) || (Build.MODEL.compareToIgnoreCase("TC55") == 0) 
    			|| (Build.MODEL.compareToIgnoreCase("TC700H") == 0) || (Build.MODEL.compareToIgnoreCase("TC75") == 0) ){
    		flag = true;
    		
    	}
		return flag;
    }
	
	public static String getTimezoneStr() {
		Calendar cal = Calendar.getInstance();
		TimeZone tz = cal.getTimeZone();
		return tz.getDisplayName();
	}

    public void resolveAppName(String appName, Intent intent) throws IllegalArgumentException {
        PackageManager mgr = getPackageManager();
        PackageInfo info;
        try {
            info = mgr.getPackageInfo(appName, PackageManager.GET_ACTIVITIES);
        } catch (NameNotFoundException e) {
            throw new RuntimeException(e);
        }
        if (info.activities.length == 0) {
            throw new IllegalArgumentException("Application activities were not found: " + appName);
        }
        String className = null;
        {
            Intent main_intent = new Intent(Intent.ACTION_MAIN);
            //main_intent.addCategory(Intent.CATEGORY_DEFAULT);
            //main_intent.addCategory(Intent.CATEGORY_LAUNCHER);
            
            List<ResolveInfo> activities = mgr.queryIntentActivities(main_intent, 0/*PackageManager.MATCH_DEFAULT_ONLY*/);
            
            Iterator<ResolveInfo> activities_iterator = activities.iterator();
            while (activities_iterator.hasNext()) {
                ResolveInfo resolve_info = activities_iterator.next();
                //Utils.platformLog("$$$", "     ACTIVITY NAME["+resolve_info.activityInfo.name+"] TARGET["+resolve_info.activityInfo.targetActivity+"]");                  
                if (appName.equals(resolve_info.activityInfo.packageName)) {
                    // we found our activity !
                    className = resolve_info.activityInfo.name;
                    break;
                }
            }
        }
        if (className == null) {
            throw new IllegalArgumentException("Application has no activities wich can handle intent with MAIN action: " + appName);
        }
        if (className.startsWith(".")) {
            className = appName + className;
        }
        intent.setClassName(appName, className);
    }

    public void parseAppParams(Object params, Intent intent) throws IllegalArgumentException {
        if (params != null) {
            Bundle startParams = new Bundle();
            if (params instanceof String) {
                if (((String) params).length() != 0) {
                    String[] paramStrings = ((String) params).split("&");
                    for (int i = 0; i < paramStrings.length; ++i) {
                        String key = paramStrings[i];
                        String value = "";
                        int splitIdx = key.indexOf('=');
                        if (splitIdx != -1) {
                            value = key.substring(splitIdx + 1);
                            key = key.substring(0, splitIdx);
                        }
                        startParams.putString(key, value);
                    }
                }
            }
            else {
                throw new IllegalArgumentException("Unknown type of incoming parameter: " + params.getClass().getSimpleName());
            }

            intent.putExtras(startParams);
        }
    }

    public static void runApplication(String appName, Object params) {
        try {
            RhodesService service = getInstance();
            Context ctx = RhodesService.getContext();

            Intent intent = service.getPackageManager().getLaunchIntentForPackage(appName);

            try {
            	service.resolveAppName(appName, intent);
            } catch ( Throwable e ) {
            	Logger.W(TAG, "Can't resolve app name, will run with default intent: " + appName);
            	Logger.W(TAG, e);
            }

            service.parseAppParams(params, intent);

            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            ctx.startActivity(intent);
        } catch (Throwable e) {
            Logger.E(TAG, "Can't run application " + appName);
            Logger.E(TAG, e);
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
				R.layout.status_bar_ongoing_event_progress_bar);
		
		StringBuilder newUrl = new StringBuilder();
		if (url.length() < 17)
			newUrl.append(url);
		else {
			newUrl.append(url.substring(0, 7));
			newUrl.append("...");
			newUrl.append(url.substring(url.length() - 7, url.length()));
		}
		expandedView.setTextViewText(R.id.title, newUrl.toString());
		
		StringBuffer downloadingText = new StringBuffer();
		if (totalBytes > 0) {
			long progress = currentBytes*100/totalBytes;
			downloadingText.append(progress);
			downloadingText.append('%');
		}
		expandedView.setTextViewText(R.id.progress_text, downloadingText.toString());
		expandedView.setProgressBar(R.id.progress_bar,
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
						});
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

    /** Opens remote or local URL
     * @throws URISyntaxException, ActivityNotFoundException */
    public static void openExternalUrl(String url) throws URISyntaxException, ActivityNotFoundException
    {
            if(url.charAt(0) == '/')
                url = "file://" + RhoFileApi.absolutePath(url);

            //FIXME: Use common URI handling
            Context ctx = RhodesService.getContext();
            LocalFileHandler fileHandler = new LocalFileHandler(ctx);
            if(!fileHandler.handle(url))
            {
                Logger.D(TAG, "Handling URI: " + url);
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setData(Uri.parse(url));
                ctx.startActivity(intent);                
            }
    }

    public native void setPushRegistrationId(String type, String id);

    private native boolean callPushCallback(String type, String json);

	private void handlePushMessage(String pushType, Bundle extras) {
		Logger.D(TAG, "Handle PUSH message");
		
		if (extras == null) {
			Logger.W(TAG, "Empty PUSH message received");
			return;
		}

        String phoneId = extras.getString("phone_id");
        if (phoneId != null && phoneId.length() > 0 &&
                !phoneId.equals(this.getPhoneId().toString())) {
            Logger.W(TAG, "Push message for another phone_id: " + phoneId);
            Logger.W(TAG, "Current phone_id: " + this.getPhoneId().toString());
            return;
        }

        String json = new JSONGenerator(extras).toString();

        Logger.D(TAG, "Received PUSH message: " + json);
        callPushCallback(pushType, json);
    }

    private void handlePushMessage(String type, String json) {
        Logger.T(TAG, "Handle push message");
        
        Logger.D(TAG, "Push message JSON: " + json);
        
        callPushCallback(type, json);
    }
    
	private void restartGeoLocationIfNeeded() {
		if (mNeedGeoLocationRestart) {
			//GeoLocation.restart();
			mNeedGeoLocationRestart = false;
		}
	}
	
	private void stopGeoLocation() {
		mNeedGeoLocationRestart = GeoLocation.isAvailable();
		//GeoLocation.stop();
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
			    });
			}
		}
		return wasEnabled;
	}
	
	private static String fetchUUID()
	{
		String uuid = "";
		// Get serial number from UUID file built into image
		try
		{
		    if (isSymbolDevice())
		    {
				BufferedReader reader = new BufferedReader(new FileReader("/sys/hardware_id/uuid"));
				uuid = reader.readLine();
				Logger.T(TAG, "uuid: " + uuid);
		    }
		    else
		    {
				uuid = computeUUID();
				Logger.T(TAG, "uuid: " + uuid);
		    }
		}
		catch (Throwable e)
		{
		    Logger.E(TAG, "Cannot determine device UUID");
		}
		return uuid;
	}
	
	/**
	 * This method is used only for non-Symbol devices as the UUID needs to be computed by other parameters.
	 * @return 32-byte long UUID
	 */
	private static String computeUUID()
	{
	    RhodesService srv = RhodesService.getInstance();
        if (srv == null)
			throw new IllegalStateException("No rhodes service instance at this moment");
		String res="";
	    WifiManager wifi = (WifiManager) srv.getSystemService(Context.WIFI_SERVICE);
	    // Get WiFi status
	    WifiInfo wifiInfo = wifi.getConnectionInfo();
	    String macAddress = wifiInfo.getMacAddress();
	    macAddress = macAddress.replaceAll(":", "");
	    UUID localUuid = UUID.nameUUIDFromBytes(macAddress.getBytes());
	    res = localUuid.toString().replaceAll("-", "");
	    return res.toUpperCase();
	}
	
	void handleAppActivation() {
		if (DEBUG)
			Log.d(TAG, "handle app activation");
		restartGeoLocationIfNeeded();
		restoreWakeLockIfNeeded();
		callActivationCallback(true);
        RhodesApplication.stateChanged(RhodesApplication.AppState.AppActivated);
	}
	
	void handleAppDeactivation() {
		if (DEBUG)
			Log.d(TAG, "handle app deactivation");
        RhodesApplication.stateChanged(RhodesApplication.AppState.AppDeactivated);
		stopWakeLock();
		stopGeoLocation();
		callActivationCallback(false);
	}

    @Override
    public void startActivity(Intent intent) {
        RhodesActivity ra = RhodesActivity.getInstance();
        if (ra == null ||
            (intent.getComponent() != null && intent.getComponent().compareTo(new ComponentName(this, RhodesActivity.class.getName())) == 0)) {
            super.startActivity(intent);
        }
        else {
            ra.startActivity(intent);
        }
    }

    public static void setFullscreen(boolean enable) {
        BaseActivity.setFullScreenMode(enable);
    }

    public static boolean getFullscreen() {
        return BaseActivity.getFullScreenMode();
    }

    public static void setScreenAutoRotate(boolean enable) {
        BaseActivity.setScreenAutoRotateMode(enable);
    }

    public static boolean getScreenAutoRotate() {
        return BaseActivity.getScreenAutoRotateMode();
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

    public static void bringToFront() {
        if (RhodesApplication.canHandleNow(RhodesApplication.UiState.MainActivityResumed)) {
            Logger.T(TAG, "Main activity is already at front, do nothing");
            return;
        }

        RhodesService srv = RhodesService.getInstance();
        if (srv == null)
            throw new IllegalStateException("No rhodes service instance at this moment");

        Logger.T(TAG, "Bring main activity to front");

        Intent intent = new Intent();
        intent.setClassName(srv.getPackageName(), RhodesActivity.class.getCanonicalName());
        intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_FROM_BACKGROUND | Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        
        srv.startActivity(intent);
    }
    
    public static void minimize() {
        if (!RhodesApplication.canHandleNow(RhodesApplication.AppState.AppActivated)) {
            Logger.T(TAG, "Application is already deactivated, do nothing");
            return;
        }
        
        try {
            RhodesActivity.safeGetInstance().moveTaskToBack(true);
        } catch (RuntimeException ex) {
            Logger.E(TAG, "Minimize failed");
            Logger.E(TAG, ex);
        }
    }


    public static native void onUiThreadCallback(int idx);

    public static void runOnUiThread(final int idx) {
        PerformOnUiThread.exec(new Runnable() {
            public void run() {
                RhodesService.onUiThreadCallback(idx);
            }
        });
    }

    public static String getNativeMenu() {
        List<Object> menuItems = RhodesActivity.safeGetInstance().getMenu().getMenuDescription();
        String menuJson = new JSONGenerator(menuItems).toString();

        Logger.T(TAG, "Menu: " + menuJson);

        return menuJson;
    }

    public static void setNativeMenu(List<String> jsonMenu) {
        List<Map<String, String>> nativeMenu = new ArrayList<Map<String, String>>();

        Iterator<String> iter = jsonMenu.iterator();
        while(iter.hasNext()) {
            try {
                String jsonItem = iter.next();
                Map<String, String> menuItem = new HashMap<String, String>();
                JSONObject json = new JSONObject(jsonItem);
                Iterator<String> itemIter = json.keys();
                while(itemIter.hasNext()) {
                    Logger.T(TAG, "New menu item");
                    String itemKey = itemIter.next();
                    String itemDescr = json.getString(itemKey);

                    Logger.T(TAG, itemKey + "->" + itemDescr);

                    menuItem.put(itemKey, itemDescr);
                }
                nativeMenu.add(menuItem);
            } catch (JSONException e) {
                Logger.E(TAG, e);
            }
        }
        RhodesActivity.safeGetInstance().getMenu().setMenu(nativeMenu);
        if (Build.VERSION.SDK_INT >= 11) {
        	PerformOnUiThread.exec(new Runnable() {
        		public void run() {
        			RhodesActivity.safeGetInstance().invalidateOptionsMenu();
        		}
        	});
        }
    }

   public static String getLocalIpAddress()
{
   String res = "";
    try {
        for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
            NetworkInterface intf = en.nextElement();
            for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                InetAddress inetAddress = enumIpAddr.nextElement();
                if (!inetAddress.isLoopbackAddress() && (inetAddress instanceof Inet4Address)) {
                    res = inetAddress.getHostAddress().toString();
                    //Logger.T(TAG, "@@@@@@@@@@@@@@@@ = "+res);
                }
             }
         }
     } catch (SocketException ex) {
         //Log.e(LOG_TAG, ex.toString());
     }

     return res;
}


    public static void removeSplashScreen() {
        getInstance().getMainView().removeSplashScreen();
    }

    public static Map getSystemInfo() {
    	Map ret = new HashMap<String,String>();

    	try {
    		ret.put( "build.serial", 				Build.SERIAL );
    		ret.put( "build.model", 				Build.MODEL );
    		ret.put( "build.id", 					Build.ID );
    		ret.put( "build.manufacturer", 			Build.MANUFACTURER );
    		ret.put( "build.brand", 				Build.BRAND );
    		ret.put( "build.type", 					Build.TYPE );
    		ret.put( "build.user", 					Build.USER );
    		ret.put( "build.board", 				Build.BOARD );
    		ret.put( "build.host", 					Build.HOST );
    		ret.put( "build.fingerprint", 			Build.FINGERPRINT );
    		ret.put( "build.version.release",		Build.VERSION.RELEASE );
    		ret.put( "build.version.incremental", 	Build.VERSION.INCREMENTAL );    		
    		ret.put( "build.version.codename",		Build.VERSION.CODENAME );
    		ret.put( "build.version.sdk",			String.valueOf(Build.VERSION.SDK_INT) );
    		ret.put( "build.bootloader",			Build.BOOTLOADER );    		
    		ret.put( "build.device",				Build.DEVICE );
    		ret.put( "build.display",				Build.DISPLAY );
    		ret.put( "build.hardware",				Build.HARDWARE );
    		ret.put( "build.product",				Build.PRODUCT );
    		ret.put( "build.radio",					Build.RADIO );
			//if ( Build.VERSION.SDK_INT >= 21 ) {
			//	ret.put("build.abis", TextUtils.join(",", Build.SUPPORTED_ABIS));
			//}
    		ret.put( "build.tags",					Build.TAGS );
    		ret.put( "build.type",					Build.TYPE );
    		ret.put( "build.time",					String.valueOf(Build.TIME) );
    		ret.put( "build.user",					Build.USER );

    		PackageManager pm = getContext().getPackageManager();
    		String packageName = getContext().getPackageName();
    		ApplicationInfo appInfo = pm.getApplicationInfo( packageName, 0 );
    		PackageInfo pakInfo = pm.getPackageInfo( packageName,PackageManager.GET_PERMISSIONS|PackageManager.GET_META_DATA|PackageManager.GET_CONFIGURATIONS );

    		StringBuilder sb = new StringBuilder();
			StringBuilderPrinter sbp = new StringBuilderPrinter(sb);
			appInfo.dump( sbp, "" );
			ret.put( "app.info", sb.toString() );
    		ret.put( "package.name", packageName );
    		ret.put( "package.version", pakInfo.versionName );
    		ret.put( "package.versioncode", String.valueOf( pakInfo.versionCode ));
			ret.put( "package.permissions", TextUtils.join(",",pakInfo.requestedPermissions) );

			List<String> pis = new ArrayList<String>();

			if ( pakInfo.reqFeatures != null ) {
				for (FeatureInfo fi : pakInfo.reqFeatures ) {
					pis.add( fi.name + ":flags" + fi.flags + ":gl" + fi.getGlEsVersion() );
				}
				ret.put( "package.features", TextUtils.join(",",pis) );
				pis.clear();
			}

    		for ( PackageInfo pi : pm.getInstalledPackages(0) ) {
    			pis.add( pi.packageName + ":" + pi.versionName );
			}
			ret.put("packages", TextUtils.join(",",pis) );

			ConnectivityManager cm = (ConnectivityManager) getContext().getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo ni = cm.getActiveNetworkInfo();

			if ( ni != null ) {
				ret.put("network.detailedstate", ni.getDetailedState().toString());
				ret.put("network.extrainfo", ni.getExtraInfo());
				ret.put("network.failreason", ni.getReason());
				ret.put("network.state", ni.getState().toString());
				ret.put("network.subtype", ni.getSubtypeName());
				ret.put("network.type", ni.getTypeName());
				ret.put("network.available", String.valueOf(ni.isAvailable()));
				ret.put("network.connected", String.valueOf(ni.isConnected()));
				ret.put("network.connectedorconnecting", String.valueOf(ni.isConnectedOrConnecting()));
				ret.put("network.failover", String.valueOf(ni.isFailover()));
				ret.put("network.roaming", String.valueOf(ni.isRoaming()));
			}

			//TODO: for future use
			//if ( Build.VERSION.SDK_INT >= 23 ) {
			//	ret.put( "network.proxy", cm.getDefaultProxy().getHost() );
			//	LinkProperties lp = cm.getLinkProperties( cm.getActiveNetwork() );
			//}

			TelephonyManager tm = (TelephonyManager)getContext().getSystemService(Context.TELEPHONY_SERVICE);
			ret.put("cell.dataactivity", String.valueOf(tm.getDataActivity()) );
			//ret.put("cell.datanetworktype", String.valueOf(tm.getDataNetworkType()));
			ret.put("cell.datastate", String.valueOf(tm.getDataState()));
			//if ( Build.VERSION.SDK_INT < 26 ) {
			//	ret.put("cell.deviceid", String.valueOf(tm.getDeviceId()));
			//}
			ret.put("cell.operator", tm.getNetworkOperatorName() );
			ret.put("cell.country", tm.getNetworkCountryIso() );
			ret.put("cell.networktype", String.valueOf(tm.getNetworkType()));
			ret.put("cell.phonetype", String.valueOf(tm.getPhoneType()));

			ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
			ActivityManager activityManager = (ActivityManager) getContext().getSystemService(ACTIVITY_SERVICE);
			activityManager.getMemoryInfo(mi);
			double availableMegs = mi.availMem / 0x100000L;

			ret.put("memory.total", String.valueOf(mi.availMem) );
			ret.put("memory.available", String.valueOf(mi.totalMem) );
			ret.put("memory.threshold", String.valueOf(mi.threshold) );
			ret.put("memory.islowmem", String.valueOf(mi.lowMemory) );
			ret.put("memory.percentavailable", String.valueOf(mi.availMem / (double)mi.totalMem * 100.0) );

			Runtime runtime = Runtime.getRuntime();
			ret.put("memory.runtimefree", String.valueOf(runtime.freeMemory()) );
			ret.put("memory.runtimemax", String.valueOf(runtime.maxMemory()) );
			ret.put("memory.runtimetotal", String.valueOf(runtime.totalMemory()) );

    	} catch ( Exception e ) {
    		ret.put( "error", e.toString() );
    	}

    	return ret;
    }
    
}

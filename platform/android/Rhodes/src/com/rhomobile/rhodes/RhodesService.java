package com.rhomobile.rhodes;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import com.rhomobile.rhodes.mainview.MainView;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class RhodesService extends Service {
	
	private static final String TAG = RhodesService.class.getName();
	
	private static final boolean DEBUG = false;
	
	private static RhodesService sInstance;
	
	private final IBinder mBinder = new LocalBinder();
	
	@SuppressWarnings("rawtypes")
	private static final Class[] mStartForegroundSignature = new Class[] {int.class, Notification.class};
	@SuppressWarnings("rawtypes")
	private static final Class[] mStopForegroundSignature = new Class[] {boolean.class};
	
	private Method mStartForeground;
	private Method mStopForeground;
	
	private NotificationManager mNM;
	
	private MainView mMainView;
	
	private RhodesAppOptions mRhodesAppOptions;
	
	public static RhodesService getInstance() {
		return sInstance;
	}
	
	public RhodesAppOptions getOptions() {
		return mRhodesAppOptions;
	}
	
	public class LocalBinder extends Binder {
		RhodesService getService() {
			return RhodesService.this;
		}
	};

	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}
	
	@Override
	public void onCreate() {
		sInstance = this;
		
		try {
			mStartForeground = getClass().getMethod("startForeground", mStartForegroundSignature);
			mStopForeground = getClass().getMethod("stopForeground", mStopForegroundSignature);
		}
		catch (NoSuchMethodException e) {
			mStartForeground = null;
			mStopForeground = null;
		}
		
		mNM = (NotificationManager)getApplicationContext().getSystemService(Context.NOTIFICATION_SERVICE);
		
		mRhodesAppOptions = new RhodesAppOptions();
	}
	
	@Override
	public void onDestroy() {
		sInstance = null;
	}
	
	@Override
	public void onStart(Intent intent, int startId) {
		handleCommand(intent);
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		handleCommand(intent);
		return Service.START_STICKY;
	}
	
	private void handleCommand(Intent intent) {
		// TODO:
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
		
		setForeground(true);
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
		setForeground(false);
	}
	
	public void setMainView(MainView v) {
		mMainView = v;
	}
	
	public MainView getMainView() {
		return mMainView;
	}

	public void doSyncAllSources(boolean v) {
		// TODO: avoid RhoService
		RhoService.getInstance().doSyncAllSources(v);
	}
	
	public String normalizeUrl(String url) {
		// TODO: avoid RhoService
		return RhoService.getInstance().normalizeUrl(url);
	}
}

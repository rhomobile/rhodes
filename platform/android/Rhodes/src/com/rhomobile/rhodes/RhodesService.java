package com.rhomobile.rhodes;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

public class RhodesService extends Service {
	
	private static boolean created = false;
	
	public static boolean isCreated() {
		return created;
	}
	
	public class LocalBinder extends Binder {
		public RhodesService getService() {
			return RhodesService.this;
		}
	};
	
	private LocalBinder mBinder = new LocalBinder();
	
	@Override
	public void onCreate() {
		created = true;
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		return mBinder;
	}

}

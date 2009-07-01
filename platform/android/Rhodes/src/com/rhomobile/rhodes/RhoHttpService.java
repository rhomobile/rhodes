package com.rhomobile.rhodes;

import com.rho.RhoRuby;
import com.rhomobile.rhodes.http.HttpServer;
import com.rhomobile.rhodes.http.ThreadedServer;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class RhoHttpService extends Service {
	
	HttpServer httpServer = null;
	
	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		
		httpServer = new HttpServer();
	}

	@Override
	public void onStart(Intent intent, int startId) {
		synchronized (this) {
			// Start HttpServer
			try {
				//SyncUtil.init();
				
				Log.d("RhoHttpService", "start http server...");
				httpServer.configure(new RhoHttpHandler());
				httpServer.start();
			} catch (Exception e) {
				httpServer = null;
				Log.e(this.getClass().getSimpleName(), e.getMessage());
			}
		}
	}

	@Override
	public void onDestroy() {
		httpServer.stop();
		
		super.onDestroy();
	}

}

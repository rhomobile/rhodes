package com.rhomobile.rhodes;

import com.rho.RhoRuby;
import com.rhomobile.rhodes.http.HttpServer;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class RhoHttpService extends Service {
	
	HttpServer httpServer = new HttpServer();

	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
	}

	@Override
	public void onStart(Intent intent, int startId) {
		// Start HttpServer
		try {
			//SyncUtil.init();
			
			httpServer.configure(new RhoHttpHandler());
			httpServer.start();
		} catch (Exception e) {
			Log.e(this.getClass().getSimpleName(), e.getMessage());
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy();

		httpServer.stop();

		RhoRuby.RhoRubyStop();
	}

}

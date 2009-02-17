package com.rhomobile.rhodes;

import com.rho.RhoRuby;
import com.rhomobile.rhodes.http.HttpServer;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.util.Log;

public class RhoHttpService extends Service {

	private final static int HTTP_SERVER_STARTED = 0;
	
	HttpServer httpServer = new HttpServer();

	/**
	 * The value returned from the
	 */
	private int value = 0;

	/**
	 * The list of all available callbacks
	 */
	private final RemoteCallbackList<ICallback> callbacks = new RemoteCallbackList<ICallback>();

	@Override
	public IBinder onBind(Intent arg0) {
		return binder;
	}

	private final IService.Stub binder = new IService.Stub() {

		/**
		 * The service value
		 * 
		 * @return the service value
		 */
		public int getValue() throws RemoteException {
			return value;
		}

		/**
		 * Set the value of the service
		 * 
		 * @param value
		 *            the service value
		 */
		public void setValue(int val) throws RemoteException {
			value = val;

			final int nCallbacks = callbacks.beginBroadcast();
			for (int i = 0; i < nCallbacks; i++) {
				try {
					callbacks.getBroadcastItem(i).valueChanged(value);
				} catch (RemoteException e) {
					e.printStackTrace();
				}
			}
			callbacks.finishBroadcast();
		}

		public void registerCallback(ICallback cb) throws RemoteException {
			if (cb != null) {
				callbacks.register(cb);
			}

		}

		public void unregisterCallback(ICallback cb) throws RemoteException {
			if (cb != null) {
				callbacks.unregister(cb);
			}

		}

	};

	@Override
	public void onCreate() {
		super.onCreate();

		RhoRuby.setRhoRubyImpl(new RhoRubyImpl());
	}

	@Override
	public void onStart(Intent intent, int startId) {
		// Start HttpServer
		try {
			RhoRuby.RhoRubyStart("");

			httpServer.configure(new RhoHttpHandler());
			httpServer.start();
			
			binder.setValue(HTTP_SERVER_STARTED); //send event to activity
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

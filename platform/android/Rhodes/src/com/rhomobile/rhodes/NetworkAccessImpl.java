package com.rhomobile.rhodes;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import android.util.Log;

import com.rho.net.IHttpConnection;
import com.rho.net.INetworkAccess;

public class NetworkAccessImpl implements INetworkAccess {

	private NetworkStateTracker networkStateTracker;

	public NetworkAccessImpl() {
		RhodesInstance.getInstance().runOnUiThread(new Runnable() {

			public void run() {
				try {
					networkStateTracker = new NetworkStateTracker(RhodesInstance.getInstance());
					networkStateTracker.enable();
				}catch (Exception e){
					Log.e( "NetworkAccessImpl", e.getMessage());
				}
				
			}
	    	
	    });

	}

	public void autoConfigure() {
	}

	public IHttpConnection connect(String server) throws IOException {
		
		int fragment = server.indexOf('#');
		if (-1 != fragment) {
			server = server.substring(0, fragment);
		}
		
		URL url = new URL(server);
		
		HttpURLConnection urlc = (HttpURLConnection)url.openConnection();
		urlc.setDoOutput(true);
		urlc.setDoInput(true);
		urlc.setUseCaches(false);
		urlc.setAllowUserInteraction(false);
		urlc.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
		return new AndroidHttpConnection(urlc);
	}

	public String getSuffix() {
		return "";
	}

	public synchronized boolean isNetworkAvailable() {
		if ( networkStateTracker != null )
			return networkStateTracker.isNetworkConnected();
		else
			return true;
	}

	public void log(String txt) {
		HttpLog.v(txt);
	}

	public void close() {
		
	}

	public void configure() {
		
	}

}

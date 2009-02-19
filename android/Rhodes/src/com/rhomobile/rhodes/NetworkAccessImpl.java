package com.rhomobile.rhodes;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import android.content.Context;

import com.rho.INetworkAccess;

public class NetworkAccessImpl implements INetworkAccess {

	private NetworkStateTracker networkStateTracker;

	public NetworkAccessImpl(Context context) {
		networkStateTracker = new NetworkStateTracker(context);
		networkStateTracker.enable();
	}

	public void autoConfigure() {
	}

	public HttpURLConnection connect(String server) throws IOException {
		
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
		urlc.setRequestProperty("Content-type", "application/x-www-form-urlencoded; charset=" + "UTF-8");
		return urlc;
	}

	public String getSuffix() {
		return "";
	}

	public synchronized boolean isNetworkAvailable() {
		return networkStateTracker.isNetworkConnected();
	}

	public void log(String txt) {
		HttpLog.v(txt);
	}

}

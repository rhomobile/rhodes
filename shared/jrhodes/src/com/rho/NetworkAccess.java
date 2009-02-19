package com.rho;

import java.io.IOException;
import java.net.HttpURLConnection;

public class NetworkAccess {

	private static INetworkAccess networkAccess;

	/**
	 * Call this method to specify platform specific RhoRuby implementation
	 * 
	 * @param networkAccessImpl
	 */
	static public void setNetworkAccessImpl(INetworkAccess networkAccessImpl) {
		if (networkAccess == null)
			networkAccess = networkAccessImpl;
	}

	static public void autoConfigure() {
		if (networkAccess != null)
			networkAccess.autoConfigure();
	}

	static public HttpURLConnection connect(String url) throws IOException {
		if (networkAccess != null)
			return networkAccess.connect(url);
		
		return null;
	}

	static public String getSuffix() {
		if (networkAccess != null)
			return networkAccess.getSuffix();
		return null;
	}

	static public boolean isNetworkAvailable() {
		if (networkAccess != null)
			return networkAccess.isNetworkAvailable();
		return false;
	}

	static public void log(String txt) {
		if (networkAccess != null)
			networkAccess.log(txt);
	}

}

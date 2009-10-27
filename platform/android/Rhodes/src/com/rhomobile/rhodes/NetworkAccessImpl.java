package com.rhomobile.rhodes;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import com.rho.net.IHttpConnection;
import com.rho.net.INetworkAccess;
import com.rho.net.RhoConnection;
import com.rho.net.URI;
import com.rhomobile.rhodes.http.HttpHeader;

public class NetworkAccessImpl implements INetworkAccess {

	public NetworkAccessImpl() {
	}

	public void autoConfigure() {
	}

	public String getHomeUrl()
	{
		return RhodesInstance.getInstance().getHomeUrl();
	}
	
	/*
	public boolean doLocalRequest(String strUrl, String strBody)
	{
		HttpHeader headers = new HttpHeader();
		headers.setHeader("Content-Type", "application/x-www-form-urlencoded");
		
		RhodesInstance.getInstance().postUrl(strUrl, strBody, headers);
		
		return true;
	}
	*/
	
	public IHttpConnection connect(String server) throws IOException {
		
		int fragment = server.indexOf('#');
		if (-1 != fragment) {
			server = server.substring(0, fragment);
		}
		
		if (URI.isLocalHost(server)) {
			URI uri = new URI(server);
			return new RhoConnection(uri);
		}
		
		URL url = new URL(server);
		
		HttpURLConnection urlc = (HttpURLConnection)url.openConnection();
		urlc.setDoOutput(true);
		urlc.setDoInput(true);
		urlc.setUseCaches(false);
		urlc.setAllowUserInteraction(false);
		urlc.setInstanceFollowRedirects(true);
		return new AndroidHttpConnection(urlc);
	}

	public String getSuffix() {
		return "";
	}

	public void log(String txt) {
		HttpLog.v(txt);
	}

	public void close() {
		
	}

	public void configure() {
		
	}

}

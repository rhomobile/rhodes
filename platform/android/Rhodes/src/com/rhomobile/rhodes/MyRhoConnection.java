package com.rhomobile.rhodes;

import java.io.ByteArrayInputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.util.Enumeration;

import android.util.Log;

import com.rho.IHttpConnection;
import com.rho.NetworkAccess;
import com.rho.RhoConnection;
import com.rhomobile.rhodes.geolocation.GeoLocation;
import com.rhomobile.rhodes.http.HttpHeader;

public class MyRhoConnection extends RhoConnection {

	public MyRhoConnection(String uri) {
		super(uri);
	}

	@Override
	protected void showGeoLocation() {
		String location;
		if( !GeoLocation.isKnownPosition() )
			location = "reading...;reading...;reading...";//<br/>" + GeoLocation.getLog();
		else
		{
			double latitude = GeoLocation.GetLatitude();
			double longitude = GeoLocation.GetLongitude();
		
			location = String.valueOf(Math.abs(latitude)) + "f° " +
				(latitude < 0 ? "South" : "North") + ", " +
				String.valueOf(Math.abs(longitude)) + "f° " +	
				(longitude < 0 ? "West" : "East") + ";" +
				String.valueOf(latitude) + ";" +
				String.valueOf(longitude) + ";";
		}
			
		responseCode = HTTP_OK;
		responseMsg = "Success";
		contentLength = 0;
		
		contentLength = location.length();
		responseData = new ByteArrayInputStream(location.getBytes());
		resHeaders.addProperty("Content-Type", "text/html" );
		resHeaders.addProperty("Content-Length", Integer.toString( contentLength ) );
	}

    @SuppressWarnings("unchecked")
	public static HttpURLConnection makeConnection(String url,
			HttpHeader requestHeaders, byte[] postData) {

    	HttpURLConnection conn = null;
		OutputStream out = null;

		try {
			conn = NetworkAccess.connect(url);
				
			if (requestHeaders != null) {
				// From
				// http://www.w3.org/Protocols/rfc2616/rfc2616-sec15.html#sec15.1.3
				//
				// Clients SHOULD NOT include a Referer header
				// field in a (non-secure) HTTP request if the
				// referring page was transferred with a secure
				// protocol.
				String referer = requestHeaders.getHeader("Referer");
				boolean sendReferrer = true;
				if (referer != null
						&& referer.toLowerCase().equals("https:")
						&& !url.toLowerCase().equals("https:")) {
					sendReferrer = false;
				}

				for (Enumeration e = requestHeaders.getHeaderKeys() ; e.hasMoreElements() ;) {
					 String key = (String)e.nextElement();

					 // remove referer header if needed
					if (!sendReferrer && key.equalsIgnoreCase("referer")) {
						continue;
					}

			         conn.setRequestProperty(key, requestHeaders.getHeader( key ) );
			     }
			}

			if (postData == null) {
				conn.setRequestMethod(IHttpConnection.GET);
			} else {
				conn.setRequestMethod(IHttpConnection.POST);

				conn.setRequestProperty("Content-Length", String.valueOf(postData.length));

				out = conn.getOutputStream();
				out.write(postData);

			}

		} catch (Exception e1) {
			Log.e("RhoConnection", e1.getMessage());
		} finally {
			if (out != null) {
				try {
					out.close();
				} catch (Exception e2) {
				}
			}
		}

		return conn;
	}

}

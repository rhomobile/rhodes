package com.rhomobile.rhodes;

import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.util.Enumeration;

import android.util.Log;

import com.rho.IHttpConnection;
import com.rho.NetworkAccess;
import com.rhomobile.rhodes.http.HttpHeader;

class ResourceFetchThread extends Thread {

    private byte[] _postData;

    private HttpHeader _requestHeaders;

    private String _url;

    public ResourceFetchThread(String url, HttpHeader requestHeaders, byte[] postData) {

        _url = url;
        _requestHeaders = requestHeaders;
        _postData = postData;
    }

    public void run() {
    	HttpURLConnection connection = MyRhoConnection.makeConnection(_url, _requestHeaders, _postData);
    	int code;
		try {
			code = connection.getResponseCode();
			if (code != IHttpConnection.HTTP_OK) {
				Log.e(getClass().getSimpleName(), connection.getResponseMessage());
			}
		} catch (Exception e) {
			Log.e(getClass().getSimpleName(), e.getMessage() );
		}
		finally {
			if ( connection != null )
				connection.disconnect();
		}
    }
    

}


package com.rhomobile.rhodes;

import java.io.IOException;

import android.util.Log;

import com.rhomobile.rhodes.http.HttpRequest;
import com.rhomobile.rhodes.http.HttpResponse;
import com.rhomobile.rhodes.http.HttpServer;
import com.rhomobile.rhodes.http.IHttpHandler;

public class RhoHttpHandler implements IHttpHandler {

	private final static String RHO_HTTP_HANDLER = "RhoHttpHandler";

	public void destroy() {
		Log.i(RHO_HTTP_HANDLER, "destroy");
	}

	public String getName() {
		return RHO_HTTP_HANDLER;
	}

	public void handle(HttpRequest request, HttpResponse response) {
		Log.i(RHO_HTTP_HANDLER, "handle request");
		try {

			HttpConnection conn = new HttpConnection(request.getRequestURI());
			
			conn.setRequestMethod(request.getMethod());

			conn.processRequest();

			response.setStatus(conn.getResponseCode(), conn.getResponseMessage());
			
			int index = 0;
			String key = null;
			
			do
			{
				key = conn.getHeaderFieldKey(index);
				String value = conn.getHeaderField(index);
			
				if ( value != null )
					response.setHeader(key, value);
				
				index++;
			}
			while (key != null);
			
		} catch (IOException e) {
			Log.e(this.getClass().getSimpleName(), e.getMessage());
		}

	}

	public void setServer(HttpServer server) throws Exception {
		Log.i(RHO_HTTP_HANDLER, "set server");
	}

	public String translate(String path) {
		Log.i(RHO_HTTP_HANDLER, "translate path: " + path);
		return path;
	}

}

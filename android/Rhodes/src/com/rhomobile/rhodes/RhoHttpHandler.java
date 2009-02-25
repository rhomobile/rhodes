package com.rhomobile.rhodes;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Enumeration;

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

	@SuppressWarnings("unchecked")
	public void handle(HttpRequest request, HttpResponse response) {
		Log.i(RHO_HTTP_HANDLER, "handle request");
		try {

			MyRhoConnection conn = new MyRhoConnection(request.getRequestURI());
			
			conn.setRequestMethod(request.getMethod());

			if ( request.getContentLength() > 0 )
				HttpResponse.copy(request.getInputStream(), conn.openOutputStream());
			
			//set request headers
			
			for (Enumeration e = request.getHeaderKeys() ; e.hasMoreElements() ;) {
				 String key = (String)e.nextElement();
		         conn.setRequestProperty(key, request.getHeader( key ) );
		     }
			
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
			
			response.writeHeaders();
			response.writeInputStream(conn.openInputStream(), conn.getLength());
		} 
		catch ( FileNotFoundException fe)
		{
			Log.e(this.getClass().getSimpleName(), fe.getMessage());
			
			try {
				response.sendError(404);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		catch (IOException e) {
			Log.e(this.getClass().getSimpleName(), e.getMessage());
			
			try {
				response.sendError(500);
			} catch (IOException e1) {
				e1.printStackTrace();
			}
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

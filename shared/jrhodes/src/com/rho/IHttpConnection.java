package com.rho;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface IHttpConnection {

	//HTTP METHODS
	public final static String GET  = "GET";
	public final static String POST = "POST";
	public final static String HEAD = "HEAD";
	
	//HTTP RESPONSE CODES
	public static int HTTP_OK = 200;
	public static int HTTP_MOVED_TEMPORARILY = 302;
	public static int HTTP_MOVED_PERMANENTLY = 301;
	public static int HTTP_BAD_REQUEST = 400;	
	public static int HTTP_NOT_FOUND = 404;
	public static int HTTP_UNAUTHORIZED = 401;
	public static int HTTP_INTERNAL_ERROR = 500;
	

	public abstract String getHost();

	public abstract String getRequestMethod();

	public abstract String getRequestProperty(String key);

	public abstract int getResponseCode();

	public abstract String getResponseMessage();

	public abstract void setRequestMethod(String method);

	public abstract long getLength();

	public abstract DataInputStream openDataInputStream();

	public abstract InputStream openInputStream();

	public abstract void close();

	public abstract DataOutputStream openDataOutputStream();

	public abstract OutputStream openOutputStream();

	public abstract void processRequest();

	public abstract String getHeaderField(int n);
	
	public abstract String getHeaderFieldKey(int n);
	
	public abstract void setRequestProperty(String key, String value); 
}
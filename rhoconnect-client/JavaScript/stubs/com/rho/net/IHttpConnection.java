package com.rho.net;

public interface IHttpConnection {

	//HTTP METHODS
	public final static String GET  = "GET";
	public final static String POST = "POST";
	public final static String HEAD = "HEAD";
	
	//HTTP RESPONSE CODES
	public static int HTTP_OK = 200;
	public static int HTTP_PARTIAL_CONTENT = 206;
	public static int HTTP_MOVED_TEMPORARILY = 302;
	public static int HTTP_MOVED_PERMANENTLY = 301;
	public static final int HTTP_MOVED_PERM = 301;
	public static int HTTP_BAD_REQUEST = 400;	
	public static int HTTP_NOT_FOUND = 404;
	public static int HTTP_UNAUTHORIZED = 401;
	public static int HTTP_RANGENOTSATISFY = 416;
	public static int HTTP_INTERNAL_ERROR = 500;
	public static int HTTP_NOTMODIFIED = 304;
}
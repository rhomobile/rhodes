/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rho.net;

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

	public abstract String getHost();

	public abstract String getRequestMethod();

	public abstract String getRequestProperty(String key);

	public abstract int getResponseCode()throws IOException;

	public abstract String getResponseMessage()throws IOException;

	public abstract void setRequestMethod(String method)throws IOException;

	public abstract long getLength();

	public abstract DataInputStream openDataInputStream()throws IOException;

	public abstract InputStream openInputStream()throws IOException;

	public abstract void close()throws IOException;

	public abstract DataOutputStream openDataOutputStream()throws IOException;

	public abstract OutputStream openOutputStream()throws IOException;

	public abstract String getHeaderField(int n)throws IOException;
	
	public abstract String getHeaderFieldKey(int n)throws IOException;
	
	public abstract void setRequestProperty(String key, String value)throws IOException;
	
	public abstract Object getNativeConnection();
	
	public abstract long getDate() throws IOException;
	public abstract long getExpiration() throws IOException;
	public abstract String getFile();
	public abstract String getHeaderField(String name) throws IOException;
	public abstract long getHeaderFieldDate(String name, long def) throws IOException;
	public abstract int getHeaderFieldInt(String name, int def) throws IOException;
	public abstract long getLastModified() throws IOException;
	public abstract int getPort();
	public abstract String getProtocol();
	public abstract String getQuery();
	public abstract String getRef();
	public abstract String getURL();
	public abstract String getEncoding();
	public abstract String getType();

}
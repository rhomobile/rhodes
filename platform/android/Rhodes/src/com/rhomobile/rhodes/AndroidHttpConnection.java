package com.rhomobile.rhodes;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.util.Enumeration;

import android.util.Log;

import com.rho.net.IHttpConnection;
import com.rhomobile.rhodes.http.HttpHeader;

public class AndroidHttpConnection implements IHttpConnection {

	HttpURLConnection urlc;
	
	public AndroidHttpConnection(HttpURLConnection urlc){
		this.urlc = urlc;
	}
	
	public void close() throws IOException {
		urlc.disconnect();
	}

	public long getDate() throws IOException {
		return urlc.getDate();
	}

	public String getEncoding() {
		return urlc.getContentEncoding();
	}

	public long getExpiration() throws IOException {
		return urlc.getExpiration();
	}

	public String getFile() {
		return urlc.getURL().getFile();
	}

	public String getHeaderField(int n) throws IOException {
		return urlc.getHeaderField(n);
	}

	public String getHeaderField(String name) throws IOException {
		return urlc.getHeaderField(name);
	}

	public long getHeaderFieldDate(String name, long def) throws IOException {
		return urlc.getHeaderFieldDate(name, def);
	}

	public int getHeaderFieldInt(String name, int def) throws IOException {
		return urlc.getHeaderFieldInt(name, def);
	}

	public String getHeaderFieldKey(int n) throws IOException {
		return urlc.getHeaderFieldKey(n);
	}

	public String getHost() {
		return urlc.getURL().getHost();
	}

	public long getLastModified() throws IOException {
		return urlc.getLastModified();
	}

	public long getLength() {
		return urlc.getContentLength();
	}

	public Object getNativeConnection() {
		return urlc;
	}

	public int getPort() {
		return urlc.getURL().getPort();
	}

	public String getProtocol() {
		return urlc.getURL().getProtocol();
	}

	public String getQuery() {
		return urlc.getURL().getQuery();
	}

	public String getRef() {
		return urlc.getURL().getRef();
	}

	public String getRequestMethod() {
		return urlc.getRequestMethod();
	}

	public String getRequestProperty(String key) {
		return urlc.getRequestProperty(key);
	}

	public int getResponseCode() throws IOException {
		return urlc.getResponseCode();
	}

	public String getResponseMessage() throws IOException {
		return urlc.getResponseMessage();
	}

	public String getType() {
		return urlc.getContentType();
	}

	public String getURL() {
		return urlc.getURL().toExternalForm();
	}

	public DataInputStream openDataInputStream() throws IOException {
		return new DataInputStream(openInputStream());
	}

	public DataOutputStream openDataOutputStream() throws IOException {
		return new DataOutputStream(openOutputStream());
	}

	public InputStream openInputStream() throws IOException {
		return urlc.getInputStream();
	}

	public OutputStream openOutputStream() throws IOException {
		return urlc.getOutputStream();
	}

	public void setRequestMethod(String method) throws IOException {
		urlc.setRequestMethod(method);
	}

	public void setRequestProperty(String key, String value) throws IOException {
		urlc.setRequestProperty(key, value);
	}

    @SuppressWarnings("unchecked")
	public static IHttpConnection makeConnection(String url,
			HttpHeader requestHeaders, byte[] postData) {

    	IHttpConnection conn = null;
		OutputStream out = null;

		try {
			NetworkAccessImpl netAccess = new NetworkAccessImpl();
			conn = netAccess.connect(url);
				
			if (requestHeaders != null) {
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

				out = conn.openOutputStream();
				out.write(postData);

			}

		} catch (Exception e1) {
			Log.e("AndroidHttpConnection", e1.getMessage());
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

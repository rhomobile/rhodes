package com.rho.sync;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javolution.io.UTF8StreamReader;

import com.rho.NetworkAccess;
import com.rho.IHttpConnection;
import java.net.HttpURLConnection;

/**
 * The Class SyncManager.
 */
public class SyncManager {

	private static HttpURLConnection connection = null;
	private static char[] m_ReadBuffer = new char[1024];
	
	private static final StringBuffer readFully(InputStream in) throws IOException {
		StringBuffer buffer = new StringBuffer();
		UTF8StreamReader reader = new UTF8StreamReader();
		reader.setInput(in);
		while (true) {
			synchronized (m_ReadBuffer) {
				int len = reader.read(m_ReadBuffer);
				if (len < 0) {
					break;
				}
				buffer.append(m_ReadBuffer, 0, len);
			}
		}
		return buffer;
	}
	
	/**
	 * Fetch remote data.
	 * 
	 * @param url
	 *            the url
	 * 
	 * @return the string
	 * 
	 * @throws IOException
	 *             Signals that an I/O exception has occurred.
	 */
	public static String fetchRemoteData(String url, String session, boolean checkSession) 
			throws IOException {
		StringBuffer buffer = new StringBuffer();
		InputStream is = null;
		int code = 0;
		if (checkSession && (session == null || session.length() == 0)) return null;
		
		try {
			long len = 0;
			//int ch = 0;
			closeConnection();
			connection = NetworkAccess.connect(url);
			if ( session != null &&  session.length() > 0 )
				connection.setRequestProperty("Cookie", session);
			
			connection.setRequestMethod(IHttpConnection.GET);
			
			//String str = connection.getRequestProperty("Cookie");
			is = connection.getInputStream();
			len = connection.getContentLength();
			code = connection.getResponseCode();
			if (code == IHttpConnection.HTTP_OK) {
				if ( len > 1024*100)
					return null;
				
				buffer = readFully(is);
				/*if (len != -1) {
					for (int i = 0; i < len; i++) {
						if ((ch = is.read()) != -1) {
							buffer.append((char) ch);
						}
					}
				} else {
					while ((ch = is.read()) != -1) {
						buffer.append((char) ch);
					}
				}*/
			} else {
				System.out.println("Error retrieving data: " + code);
				if (code == IHttpConnection.HTTP_UNAUTHORIZED) SyncUtil.logout();
				return null;
			}
		} finally {
			if (is != null) {
				is.close();
			}
			closeConnection();
		}
		return buffer.toString();
	}

	/**
	 * Push remote data.
	 * 
	 * @param url
	 *            the url
	 * @param data
	 *            the data
	 * 
	 * @return the int
	 * 
	 * @throws IOException
	 *             Signals that an I/O exception has occurred.
	 */
	public static int pushRemoteData(String url, String data, String session, boolean checkSession)
			throws IOException {
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		if (checkSession && (session == null || session.length() == 0)) return SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		try {
			makePostRequest(url,data,session);

			int code = connection.getResponseCode();
			if (code == IHttpConnection.HTTP_INTERNAL_ERROR || code == IHttpConnection.HTTP_NOT_FOUND) {
				System.out.println("Error posting data: " + code);
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
				if (code == IHttpConnection.HTTP_UNAUTHORIZED) SyncUtil.logout();
			}
		} finally {
			closeConnection();
		}

		return success;
	}
	
	public static void makePostRequest(String url, String data, String session)
			throws IOException {
		// Performs a post to url with post body provided by data
		OutputStream os = null;
		try {
			closeConnection();
			connection = NetworkAccess.connect(url);
			if ( session != null &&  session.length() > 0 )
				connection.setRequestProperty("Cookie", session);
			
			connection.setRequestMethod(IHttpConnection.POST);
			
			os = connection.getOutputStream();
			os.write(data.getBytes());
			os.flush();
		} finally {
			if (os != null) {
				os.close();
			}
		}
	}

	public static HttpURLConnection getConnection() {
		return connection;
	}

	public static void closeConnection(){
		if ( connection != null ){
			connection.disconnect();
		}
		
		connection = null;
	}
	
}

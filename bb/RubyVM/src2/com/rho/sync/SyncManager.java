/*
 *  rhodes
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.rho.sync;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

//import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;
import com.rho.NetworkAccess;

/**
 * The Class SyncManager.
 */
public class SyncManager {

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
	public static String fetchRemoteData(String url, String session) throws IOException {
		StringBuffer buffer = new StringBuffer();
		InputStream is = null;
		HttpConnection connection = null;
		int code = 0;
		try {
			long len = 0;
			int ch = 0;
			//connection = (HttpConnection) Connector.open(url);
			connection = NetworkAccess.connect(url);
			if ( session != null &&  session.length() > 0 )
				connection.setRequestProperty("Cookie", session);
			
			String str = connection.getRequestProperty("Cookie");
			is = connection.openInputStream();
			len = connection.getLength();
			code = connection.getResponseCode();
			if (code == HttpConnection.HTTP_OK) {
				if (len != -1) {
					for (int i = 0; i < len; i++) {
						if ((ch = is.read()) != -1) {
							buffer.append((char) ch);
						}
					}
				} else {
					while ((ch = is.read()) != -1) {
						buffer.append((char) ch);
					}
				}
			} else {
				System.out.println("Error retrieving data: " + code);
				return null;
			}
		} finally {
			if (is != null) {
				is.close();
			}
			if (connection != null) {
				connection.close();
			}
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
	public static int pushRemoteData(String url, String data, String session)
			throws IOException {
		HttpConnection connection = null;
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		try {
			connection = makePostRequest(url,data,session);

			int code = connection.getResponseCode();
			if (code == HttpConnection.HTTP_INTERNAL_ERROR || code == HttpConnection.HTTP_NOT_FOUND) {
				System.out.println("Error posting data: " + code);
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
			}
		} finally {
			if (connection != null) {
				connection.close();
			}
		}

		return success;
	}
	
	public static HttpConnection makePostRequest(String url, String data, String session)
			throws IOException {
		// Performs a post to url with post body provided by data
		OutputStream os = null;
		HttpConnection connection = null;
		try {
			connection = NetworkAccess.connect(url);
			os = connection.openOutputStream();
			connection.setRequestMethod(HttpConnection.POST);
			os.write(data.getBytes());
			os.flush();
		} finally {
			if (os != null) {
				os.close();
			}
		}
	
		return connection;
	}
	
}

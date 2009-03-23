/*
 *  rhodes
 *
 *  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
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
package rhomobile.sync;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

//import javax.microedition.io.Connector;
import javax.microedition.io.HttpConnection;

import javolution.io.UTF8StreamReader;

//import com.xruby.runtime.builtin.ObjectFactory;
//import com.xruby.runtime.builtin.RubyString;
//import com.xruby.runtime.lang.RubyValue;

import rhomobile.NetworkAccess;

/**
 * The Class SyncManager.
 */
public class SyncManager {

	private static HttpConnection connection = null;
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
			
		try{
			
			is = openRemoteStream( url, session, checkSession );
			if ( is == null )
				return null;
			
			long len = connection.getLength();
	
			if ( len > 1024*100)
				return null;
			
			buffer = readFully(is);
		}finally{
			if ( is != null )
				is.close();
			
			closeConnection();
		}
		
		return buffer.toString();
	}

	public static InputStream openRemoteStream(String url, String session, boolean checkSession) 
		throws IOException {
		InputStream is = null;
		int code = 0;
		if (checkSession && (session == null || session.length() == 0)) 
			return null;
		
		closeConnection();
		connection = NetworkAccess.connect(url);
		if ( session != null &&  session.length() > 0 )
			connection.setRequestProperty("Cookie", session);
		
		is = connection.openInputStream();
		code = connection.getResponseCode();
		
		if (code != HttpConnection.HTTP_OK) 
		{
			if (is != null) {
				is.close();
			}
			closeConnection();
			
			System.out.println("Error retrieving data: " + code);
			if (code == HttpConnection.HTTP_UNAUTHORIZED) 
				SyncUtil.logout();
			return null;
		}

		return is;
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
	public static int pushRemoteData(String url, InputStream data, String session, boolean checkSession,
			String contentType ) throws IOException {
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		if (checkSession && (session == null || session.length() == 0)) return SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		try {
			makePostRequest(url,data,session,contentType);

			int code = connection.getResponseCode();
			if (code == HttpConnection.HTTP_INTERNAL_ERROR || code == HttpConnection.HTTP_NOT_FOUND) {
				System.out.println("Error posting data: " + code);
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
				if (code == HttpConnection.HTTP_UNAUTHORIZED) SyncUtil.logout();
			}
		} finally {
			closeConnection();
		}

		return success;
	}

	private static final int bufferedRead(byte[] a, InputStream in) throws IOException {
		int bytesRead = 0;
		while (bytesRead < (a.length)) {
			int read = in.read(a);//, bytesRead, (a.length - bytesRead));
			if (read < 0) {
				break;
			}
			bytesRead += read;
		}
		return bytesRead;
	}
	
	static String szMultipartPrefix = 
	   "------------A6174410D6AD474183FDE48F5662FCC5\r\n"+
	   "Content-Disposition: form-data; name=\"blob\"; filename=\"doesnotmatter.png\"\r\n"+
	   "Content-Type: application/octet-stream\r\n\r\n";
	
	static String szMultipartPostfix = 
	    "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

	static String szMultipartContType = 
	    "multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5";
	
	public static void makePostRequest(String url, InputStream data, String session, 
			String contentType )throws IOException 
	{
		// Performs a post to url with post body provided by data
		OutputStream os = null;
		try {
			closeConnection();
			connection = NetworkAccess.connect(url);
			if ( session != null &&  session.length() > 0 )
				connection.setRequestProperty("Cookie", session);
			
			boolean bMultipart = false;
			if ( contentType != null && contentType.length() > 0 ){
				if ( contentType.equalsIgnoreCase("multipart/form-data")){
					connection.setRequestProperty("content-type", szMultipartContType);
					bMultipart = true;
				}
				else
					connection.setRequestProperty("content-type", contentType);
			}
			
			os = connection.openOutputStream();
			connection.setRequestMethod(HttpConnection.POST);

			if (bMultipart){
				os.write(szMultipartPrefix.getBytes(), 0, szMultipartPrefix.length());
			}
				
			synchronized (SyncUtil.m_byteBuffer) {			
				int nRead = 0;
	    		do{
	    			nRead = bufferedRead(SyncUtil.m_byteBuffer,data);
	    			if ( nRead > 0 )
	    				os.write(SyncUtil.m_byteBuffer, 0, nRead);
	    		}while( nRead > 0 );
			}

			if (bMultipart){
				os.write(szMultipartPostfix.getBytes(), 0, szMultipartPostfix.length());
			}
			
			//os.write(data);
			os.flush();
		}catch(IOException exc){
			throw exc;
		}
		finally {
			if (os != null) {
				os.close();
			}
		}
	}

	public static HttpConnection getConnection() {
		return connection;
	}

	public static void closeConnection(){
		if ( connection != null ){
			try{
				connection.close();
			}catch(IOException exc){
				System.out.println("There was an error close connection: "
						+ exc.getMessage());
			}
		}
		
		connection = null;
	}
	
}

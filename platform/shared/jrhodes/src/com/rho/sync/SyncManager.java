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
		
		try {
			
			is = openRemoteStream( url, session, checkSession );
			if ( is == null )
				return null;
			
			long len = connection.getContentLength();
			
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
		
		connection.setRequestMethod(IHttpConnection.GET);
		
		is = connection.getInputStream();
		code = connection.getResponseCode();
		
		if (code != IHttpConnection.HTTP_OK) 
		{
			if (is != null) {
				is.close();
			}
			closeConnection();
			
			System.out.println("Error retrieving data: " + code);
			if (code == IHttpConnection.HTTP_UNAUTHORIZED) 
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
	
	static String szMultipartContType = 
	    "multipart/form-data; boundary=--A6174410D6AD474183FDE48F5662FCC5";
	static String szMultipartPrefix = 
	    "--A6174410D6AD474183FDE48F5662FCC5\r\n"+
	    "Content-Disposition: form-data; name=\"blob\"\r\n"+
	    "Content-Type: application/octet-stream\r\n";
	static String szMultipartPostfix = 
	    "----A6174410D6AD474183FDE48F5662FCC5--\r\n";
	
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

			connection.setRequestMethod(IHttpConnection.POST);

			os = connection.getOutputStream();

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

	public static HttpURLConnection getConnection() {
		return connection;
	}

	public static void closeConnection(){
		if ( connection != null ){
			try{
				connection.disconnect();
			}catch(Exception exc){
				System.out.println("There was an error close connection: "
						+ exc.getMessage());
			}
		}
		
		connection = null;
	}
	
}

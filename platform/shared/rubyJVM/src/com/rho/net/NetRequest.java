package com.rho.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javolution.io.UTF8StreamReader;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.SimpleFile;
import com.rho.Tokenizer;

public class NetRequest
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Net");
	
	static final int  MAX_NETREQUEST_RETRY  = 3;
	boolean m_bCancel = false;
	
	public static interface IRhoSession
	{
		public abstract void logout()throws Exception;
		public abstract String getSession();
	}
	
	private IHttpConnection m_connection = null;
	private char[] m_charBuffer = new char[1024];
	public  byte[]  m_byteBuffer = new byte[4096];
	
	public NetResponse pullData(String strUrl, String strBody, IRhoSession oSession ) throws Exception
    {
		return doRequestTry(strUrl, strBody, oSession, true);
    }

	public NetResponse doRequestTry(String strUrl, String strBody, IRhoSession oSession, boolean bCloseConnection ) throws Exception
    {
		NetResponse resp = null;
		
		int nTry = 0;
		m_bCancel = false;
	    do
	    {
	    	try{
	    		resp = doRequest(strUrl, strBody, oSession, bCloseConnection);
	    		break;
	    	}catch(IOException exc)
	    	{
	    		if ( m_bCancel )
	    			break;
	    		if ( nTry+1 >= MAX_NETREQUEST_RETRY )
	    			throw exc;
	    	}
	        nTry++;

	    }while( true );
		
		return resp;
    }
	
	public NetResponse doRequest(String strUrl, String strBody, IRhoSession oSession, boolean bCloseConnection ) throws Exception
    {
		StringBuffer buffer = null;
		InputStream is = null;
		OutputStream os = null;
		int code = -1;
		
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl);
			
			String strSession = oSession.getSession();
			LOG.INFO("Cookie : " + (strSession != null ? strSession : "") );
			if ( strSession != null && strSession.length() > 0 )
				m_connection.setRequestProperty("Cookie", strSession );
			
			m_connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
			m_connection.setRequestProperty("Connection", "keep-alive");
			//m_connection.setRequestProperty("Accept", "application/x-www-form-urlencoded,application/json,text/html");
			
			if ( strBody != null && strBody.length() > 0 )
			{
				m_connection.setRequestMethod(IHttpConnection.POST);
				os = m_connection.openOutputStream();
				os.write(strBody.getBytes(), 0, strBody.length());
			}else
				m_connection.setRequestMethod(IHttpConnection.GET);
			
			is = m_connection.openInputStream();
			code = m_connection.getResponseCode();
			
			LOG.INFO("getResponseCode : " + code);
			
			if (code != IHttpConnection.HTTP_OK) 
			{
				LOG.ERROR("Error retrieving data: " + code);
				if (code == IHttpConnection.HTTP_UNAUTHORIZED) 
					oSession.logout();
				
				//if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
				{
					buffer = readFully(is);
					
					if ( code == IHttpConnection.HTTP_MOVED_TEMPORARILY ||
						 code == IHttpConnection.HTTP_MOVED_PERMANENTLY )
						LOG.INFO("Response body: " + buffer.toString() );
					else
						LOG.TRACE("Response body: " + buffer.toString() );
				}
			}else
			{
				long len = m_connection.getLength();
				LOG.INFO("fetchRemoteData data size:" + len );
		
				buffer = readFully(is);
				
				LOG.INFO("fetchRemoteData data readFully.");
			}

			if ( is != null )
				try{ is.close(); }catch(IOException exc){}
			if ( os != null )
				try{ os.close(); }catch(IOException exc){}
			
			if ( bCloseConnection )
				closeConnection();

		}catch(Exception e)
		{
			if ( is != null )
				try{ is.close(); }catch(IOException exc){}
			if ( os != null )
				try{ os.close(); }catch(IOException exc){}
			
			closeConnection();
			throw e;
		}
		
		return new NetResponse(buffer != null ? buffer.toString() : "", code );
    }
	
	public NetResponse pushData(String strUrl, String strBody, IRhoSession oSession)throws Exception
    {
		return doRequest(strUrl, strBody, oSession, true);
    }
	
	static class ParsedCookie {
		String strAuth = "";
		String strSession = "";
	};

	public NetResponse pullCookies(String strUrl, String strBody, IRhoSession oSession)throws Exception
    {
		NetResponse resp = null;
		try{
    		resp = doRequest/*Try*/(strUrl, strBody, oSession, false);
    		if ( resp.isOK() )
    		{
    			ParsedCookie cookie = makeCookie(m_connection);
    			resp.setCharData(cookie.strAuth + ";" + cookie.strSession + ";");
    			LOG.INFO("pullCookies: " + resp.getCharData() );
    		}
		}finally
		{
			closeConnection();
		}
		
		return resp;
    }
	
	static String szMultipartPrefix = 
		   "------------A6174410D6AD474183FDE48F5662FCC5\r\n"+
		   "Content-Disposition: form-data; name=\"blob\"; filename=\"doesnotmatter.png\"\r\n"+
		   "Content-Type: application/octet-stream\r\n\r\n";
		
	static String szMultipartPostfix = 
	    "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

	static String szMultipartContType = 
	    "multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5";

	public NetResponse pushFile( String strUrl, String strFileName, IRhoSession oSession)throws Exception
	{
		SimpleFile file = null;
		NetResponse resp = null;
		
		try{
			file = RhoClassFactory.createFile();
			file.open(strFileName, true, true);
			
			int nTry = 0;
			do{
				try{
					resp = pushFile1(strUrl, file, oSession );
					break;
				}catch(IOException e)
				{
		    		if ( nTry+1 >= MAX_NETREQUEST_RETRY )
		    			throw e;
		    	}
		        nTry++;
			}while( true );
			
		}finally{
			if ( file != null )
				try{ file.close(); }catch(IOException e){}
		}
		
		return resp;
	}
	
	private NetResponse pushFile1( String strUrl, SimpleFile file, IRhoSession oSession)throws Exception
    {
		StringBuffer buffer = null;
		InputStream is = null;
		OutputStream os = null;
		int code  = -1;
		
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl);
			
			if ( oSession != null )
			{
				String strSession = oSession.getSession();
				if ( strSession != null && strSession.length() > 0 )
					m_connection.setRequestProperty("Cookie", strSession );
			}
			
			m_connection.setRequestProperty("content-type", szMultipartContType);
			m_connection.setRequestMethod(IHttpConnection.POST);
			
			//PUSH specific
			os = m_connection.openOutputStream();
			os.write(szMultipartPrefix.getBytes(), 0, szMultipartPrefix.length());
				
			synchronized (m_byteBuffer) {			
				int nRead = 0;
	    		do{
	    			nRead = bufferedRead(m_byteBuffer,file.getInputStream());
	    			if ( nRead > 0 )
	    				os.write(m_byteBuffer, 0, nRead);
	    		}while( nRead > 0 );
			}

			os.write(szMultipartPostfix.getBytes(), 0, szMultipartPostfix.length());
			os.flush();
			//PUSH specific
			
			is = m_connection.openInputStream();
			code = m_connection.getResponseCode();
		
			LOG.INFO("getResponseCode : " + code);
			
			if (code != IHttpConnection.HTTP_OK) 
			{
				LOG.ERROR("Error retrieving data: " + code);
				if (code == IHttpConnection.HTTP_UNAUTHORIZED) 
					oSession.logout();
				
				if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
					buffer = readFully(is);
				
			}else
			{
				long len = m_connection.getLength();
				LOG.INFO("fetchRemoteData data size:" + len );
		
				buffer = readFully(is);
				
				LOG.INFO("fetchRemoteData data readFully.");
			}			
		}finally{
			try{
				if ( is != null )
					is.close();
				if (os != null)
					os.close();
				
				closeConnection();
				
			}catch(IOException exc2){}
		}
		
		return new NetResponse(buffer != null ? buffer.toString() : "", code );
    }
	
	public NetResponse pullFile( String strUrl, String strFileName, IRhoSession oSession )throws Exception
	{
		SimpleFile file = null;
		OutputStream fstream = null;
		NetResponse resp = null;
		try{
			file = RhoClassFactory.createFile();
			file.open(strFileName, false, true);
			fstream = file.getOutStream();
			
			int nTry = 0;
			do{
				try{
					resp = pullFile1( strUrl, fstream, oSession );
					break;
				}catch(IOException e)
				{
		    		if ( nTry+1 >= MAX_NETREQUEST_RETRY )
		    			throw e;
		    	}
		        nTry++;
			}while( true );
			
		}finally{
			if ( fstream != null )
				try{ fstream.close();}catch(IOException e){}
			
			if ( file != null )
				try{ file.close(); }catch(IOException e){}
		}
		
		return resp;
	}
	
	NetResponse pullFile1( String strUrl, OutputStream fstream, IRhoSession oSession )throws Exception
	{
		StringBuffer buffer = null;
		InputStream is = null;
		int code = -1;
		
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl);
			
			String strSession = oSession.getSession();
			if ( strSession != null && strSession.length() > 0 )
				m_connection.setRequestProperty("Cookie", strSession );
			
			m_connection.setRequestMethod(IHttpConnection.GET);
			
			is = m_connection.openInputStream();
			code = m_connection.getResponseCode();
			
			LOG.INFO("getResponseCode : " + code);
			
			if (code != IHttpConnection.HTTP_OK) 
			{
				LOG.ERROR("Error retrieving data: " + code);
				if (code == IHttpConnection.HTTP_UNAUTHORIZED) 
					oSession.logout();
				
				if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
					buffer = readFully(is);
			}else
			{
				//long len = connection.getLength();
				//LOG.INFO("pullFile data size:" + len );
				//int nAvail = is.available();
				boolean bReadByBytes = RhoClassFactory.createRhoRubyHelper().isSimulator();	
				synchronized (m_byteBuffer) {			
					int nRead = 0;
		    		do{
		    			if ( bReadByBytes )
		    				nRead = bufferedReadByByte(m_byteBuffer,is);
		    			else
		    				nRead = bufferedRead(m_byteBuffer,is);
		    			
		    			if ( nRead > 0 )
		    				fstream.write(m_byteBuffer, 0, nRead);
		    		}while( nRead >= 0 );
				}
				
			}

		}finally
		{
			if ( is != null )
				try{ is.close(); }catch(IOException exc){}
			
			closeConnection();
		}
		
		return new NetResponse(buffer != null ? buffer.toString() : "", code );
	}
	
    //if strUrl.length() == 0 delete all cookies if possible
	public void deleteCookie(String strUrl)
    {
    }

	public String resolveUrl(String url) throws Exception
    {
		if ( url == null || url.length() == 0 )
			return "";

		String _httpRoot = RhoClassFactory.getNetworkAccess().getHomeUrl();
		if(!_httpRoot.endsWith("/"))
			_httpRoot = _httpRoot + "/";
		url.replace('\\', '/');
		if ( !url.startsWith(_httpRoot) ){
    		if ( url.charAt(0) == '/' )
    			url = _httpRoot.substring(0, _httpRoot.length()-1) + url;
    		else
    			url = _httpRoot + url;
		}
		return url;
    }

	public void cancel()
    {
		m_bCancel = true;
		closeConnection();
    }

	private static void parseCookie(String value, ParsedCookie cookie) {
		boolean bAuth = false;
		boolean bSession = false;
		Tokenizer stringtokenizer = new Tokenizer(value, ";");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}
			
			int i = 0;
			if ( (i=tok.indexOf("auth_token=")) >= 0 )
			{
				String val = tok.substring(i+11);
				val.trim();
				if ( val.length() > 0 )
				{
					cookie.strAuth = "auth_token=" + val;
					bAuth = true;
				}
			}else if ( (i=tok.indexOf("path=")) >= 0 )
			{
				String val = tok.substring(i+6);
				val.trim();
				if ( val.length() > 0 )
				{
					if (bAuth)
						cookie.strAuth += ";path=" + val;
					else if (bSession)
						cookie.strSession += ";path=" + val;
				}
			}else if ( (i=tok.indexOf("rhosync_session=")) >= 0 )
			{
				String val = tok.substring(i+16);
				val.trim();
				if ( val.length() > 0 )
				{
					cookie.strSession = "rhosync_session=" + val;
					bSession = true;
				}
			}
			
/*			int i = tok.indexOf('=');
			String s1;
			String s2;
			if (i > 0) {
				s1 = tok.substring(0, i);
				s2 = tok.substring(i + 1);
			} else {
				s1 = tok;
				s2 = "";
			}
			s1 = s1.trim();
			s2 = s2.trim();

			if (s1.equalsIgnoreCase("auth_token") && s2.length() > 0) {
				cookie.strAuth = s1 + "=" + s2;
				bAuth = true;
			} else if (s1.equalsIgnoreCase("path") && s2.length() > 0) {
				if (bAuth)
					cookie.strAuth += ";" + s1 + "=" + s2;
				else if (bSession)
					cookie.strSession += ";" + s1 + "=" + s2;
			} else if (s1.equalsIgnoreCase("rhosync_session")
					&& s2.length() > 0) {
				cookie.strSession = s1 + "=" + s2;
				bSession = true;
			}*/

		}
	}

	private static String extractToc(String toc_name, String data) {
		int start = data.indexOf(toc_name);
		if (start != -1) {
			int end = data.indexOf(';', start);
			if (end != -1) {
				return data.substring(start, end);
			}
		}
		return null;
	}

	/*public static void TEST()
	{
		ParsedCookie cookie = new ParsedCookie();
		//parseCookie("auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT, auth_token=887b2ffd30a7b97be9a0986d7746a934421eec7d; path=/; expires=Sat, 24 Oct 2009 20:56:55 GMT, rhosync_session=BAh7BzoMdXNlcl9pZGkIIgpmbGFzaElDOidBY3Rpb25Db250cm9sbGVyOjpGbGFzaDo6Rmxhc2hIYXNoewAGOgpAdXNlZHsA--f9b67d99397fc534107fb3b7483ccdae23b4a761; path=/; expires=Sun, 10 Oct 2010 19:10:58 GMT; HttpOnly", cookie);
		parseCookie("auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT", cookie);
		parseCookie("rhosync_session=BAh7CToNcGFzc3dvcmQiFTiMYru1W11zuoAlN%2FPtgjc6CmxvZ2luIhU4jGK7tVtdc7qAJTfz7YI3Ogx1c2VyX2lkaQYiCmZsYXNoSUM6J0FjdGlvbkNvbnRyb2xsZXI6OkZsYXNoOjpGbGFzaEhhc2h7AAY6CkB1c2VkewA%3D--a7829a70171203d72cd4e83d07b18e8fcf5e2f78; path=/; expires=Thu, 02 Sep 2010 23:51:31 GMT; HttpOnly", cookie);
		
	}*/
	
	private static ParsedCookie makeCookie(IHttpConnection connection)
			throws IOException {
		ParsedCookie cookie = new ParsedCookie();

		for (int i = 0;; i++) {
			String strField = connection.getHeaderFieldKey(i);
			if (strField == null && i > 0)
				break;

			if (strField != null && strField.equalsIgnoreCase("Set-Cookie")) {
				String header_field = connection.getHeaderField(i);
				LOG.INFO("Set-Cookie: " + header_field);
				parseCookie(header_field, cookie);
				// Hack to make it work on 4.6 device which doesn't parse
				// cookies correctly
				// if (cookie.strAuth==null) {
				// String auth = extractToc("auth_token", header_field);
				// cookie.strAuth = auth;
				// System.out.println("Extracted auth_token: " + auth);
				// }
				if (cookie.strSession == null) {
					String rhosync_session = extractToc("rhosync_session",
							header_field);
					cookie.strSession = rhosync_session;
					LOG.INFO("Extracted rhosync_session: " + rhosync_session);
				}
			}
		}

		return cookie;
	}
	
	private final StringBuffer readFully(InputStream in) throws Exception 
	{
		boolean bReadByBytes = false;
		if ( RhoConf.getInstance().getInt("bb_netreadbybytes") > 0 )
			bReadByBytes = RhoClassFactory.createRhoRubyHelper().isSimulator();
		
		StringBuffer buffer = new StringBuffer();
		UTF8StreamReader reader = new UTF8StreamReader(4096,bReadByBytes);
		reader.setInput(in);
		while (true) {
			synchronized (m_charBuffer) {
				int len = reader.read(m_charBuffer);
				if (len < 0) {
					break;
				}
				buffer.append(m_charBuffer, 0, len);
			}
		}
		return buffer;
	}

	private final int bufferedRead(byte[] a, InputStream in) throws Exception {
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
	
	private final int bufferedReadByByte(byte[] a, InputStream in) throws IOException {
		int bytesRead = 0;
		while (bytesRead < (a.length)) {
			int read = in.read();// a, 0, a.length );
			if (read < 0) {
				return bytesRead > 0 ? bytesRead : -1;
			}
			a[bytesRead] = (byte)read;
			bytesRead ++;
		}
		return bytesRead;
	}
	
	public void closeConnection(){
		if ( m_connection != null ){
			try{
				m_connection.close();
			}catch(IOException exc){
				LOG.ERROR("There was an error close connection", exc);
			}
		}
		
		m_connection = null;
	}
	
}

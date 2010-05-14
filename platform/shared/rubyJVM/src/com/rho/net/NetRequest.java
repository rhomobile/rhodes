package com.rho.net;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

//import javolution.io.UTF8StreamReader;

import com.rho.RhoClassFactory;
//import com.rho.RhoConf;
import com.rho.FilePath;
import com.rho.IRhoRubyHelper;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.file.*;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

public class NetRequest
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Net");
	
	boolean m_bCancel = false;
	
	public static interface IRhoSession
	{
		public abstract void logout()throws Exception;
		public abstract String getSession();
		public abstract String getContentType();
	}
	
	public static class MultipartItem
	{
	    //mutually exclusive
	    public String m_strFilePath = "";
	    public String m_strBody = "";
	    //

	    public String m_strName = "", m_strFileName = "", m_strContentType = "";

	    public String m_strDataPrefix = "";
	};
	
	private IHttpConnection m_connection = null;
	
	private boolean m_bIgnoreSuffixOnSim = true;
	private Hashtable m_OutHeaders;
	public boolean isCancelled(){ return m_bCancel;}
	public NetResponse pullData(String strUrl, IRhoSession oSession ) throws Exception
    {
		return doRequest("GET", strUrl, "", oSession, null);
    }

	public void setIgnoreSuffixOnSim(boolean bset)
	{
		m_bIgnoreSuffixOnSim = bset;
	}

	private void writeHeaders(Hashtable headers) throws Exception
	{
		if (headers != null && headers.size() > 0)
		{
	    	Enumeration valsHeaders = headers.elements();
	    	Enumeration keysHeaders = headers.keys();
			while (valsHeaders.hasMoreElements()) 
			{
				String strName = (String)keysHeaders.nextElement();
				String strValue = (String)valsHeaders.nextElement();
				m_connection.setRequestProperty(strName,strValue);
		    }
			
		}
	}

	private void readHeaders(Hashtable headers) throws Exception
	{
		if ( headers != null )
		{
			m_OutHeaders = new Hashtable();
			for (int i = 0;; i++) {
				String strField = m_connection.getHeaderFieldKey(i);
				if (strField == null && i > 0)
					break;

				if (strField != null ) 
				{
					String header_field = m_connection.getHeaderField(i);
					m_OutHeaders.put(strField.toLowerCase(), header_field);
				}
			}
		}
	}
	
	public static void copyHashtable(Hashtable from, Hashtable to)
	{
		if ( from == null || to == null )
			return;
		
    	Enumeration valsHeaders = from.elements();
    	Enumeration keysHeaders = from.keys();
		while (valsHeaders.hasMoreElements()) 
		{
			Object key = (String)keysHeaders.nextElement();
			Object value = (String)valsHeaders.nextElement();
			to.put(key, value);
	    }
	}
	
	public NetResponse doRequest(String strMethod, String strUrl, String strBody, IRhoSession oSession, Hashtable headers ) throws Exception
    {
		String strRespBody = null;
		InputStream is = null;
		OutputStream os = null;
		int code = -1;
		
		m_bCancel = false;
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl, m_bIgnoreSuffixOnSim);
			LOG.INFO("connection done");
			
			if ( oSession != null )
			{
				String strSession = oSession.getSession();
				LOG.INFO("Cookie : " + (strSession != null ? strSession : "") );
				if ( strSession != null && strSession.length() > 0 )
					m_connection.setRequestProperty("Cookie", strSession );
			}
			
			//m_connection.setRequestProperty("Connection", "keep-alive");
			//m_connection.setRequestProperty("Accept", "application/x-www-form-urlencoded,application/json,text/html");
			
			if ( strBody != null && strBody.length() > 0 )
			{
				if ( oSession != null )
					m_connection.setRequestProperty("Content-Type", oSession.getContentType());
				else
					m_connection.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");

				writeHeaders(headers);
				LOG.INFO("writeHeaders done");
				m_connection.setRequestMethod(IHttpConnection.POST);
				
				os = m_connection.openOutputStream();
				os.write(strBody.getBytes(), 0, strBody.length());
				LOG.INFO("write body done");
			}else
			{
				writeHeaders(headers);
				m_connection.setRequestMethod(strMethod);
			}
			
			code = m_connection.getResponseCode();
			LOG.INFO("getResponseCode : " + code);
			
			is = m_connection.openInputStream();
			LOG.INFO("openInputStream done");
			
			if (code != IHttpConnection.HTTP_OK) 
			{
				LOG.ERROR("Error retrieving data: " + code);
				if (code == IHttpConnection.HTTP_UNAUTHORIZED && oSession != null) 
					oSession.logout();
				
				//if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
				{
					strRespBody = readFully(is);
					
					if ( code == IHttpConnection.HTTP_MOVED_TEMPORARILY ||
						 code == IHttpConnection.HTTP_MOVED_PERMANENTLY )
						LOG.INFO("Response body: " + strRespBody );
					else
						LOG.TRACE("Response body: " + strRespBody );
				}
			}else
			{
				long len = m_connection.getLength();
				LOG.INFO("fetchRemoteData data size:" + len );
		
				strRespBody = readFully(is);
				
				LOG.INFO("fetchRemoteData data readFully.");
			}

			readHeaders(headers);
			copyHashtable(m_OutHeaders, headers);
		}finally
		{
			if ( is != null )
				try{ is.close(); }catch(IOException exc){}
			if ( os != null )
				try{ os.close(); }catch(IOException exc){}
			
			closeConnection();
			
			m_bIgnoreSuffixOnSim = true;
		}
		
		return makeResponse(strRespBody, code );
    }
	
	private NetResponse makeResponse(String strRespBody, int nErrorCode)throws Exception
	{
		NetResponse pResp = new NetResponse(strRespBody != null ? strRespBody : "", nErrorCode );
		if (pResp.isOK())
			pResp.setCookies(makeClientCookie(m_OutHeaders));
		
		return pResp;
	}
	
	public NetResponse pushData(String strUrl, String strBody, IRhoSession oSession)throws Exception
    {
		m_bCancel = false;
		
		if ( URI.isLocalHost(strUrl) )
		{
			IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
			return helper.postUrl(strUrl,strBody);
		}
		
		return doRequest("POST", strUrl, strBody, oSession, null);
    }
	
    public NetResponse pushMultipartData(String strUrl, MultipartItem oItem, IRhoSession oSession, Hashtable/*<String,String>**/ pHeaders)throws Exception
    {
        Vector arItems = new Vector();
        arItems.addElement(oItem);

        return pushMultipartData(strUrl, arItems, oSession, pHeaders);
    }
	
	public NetResponse pullCookies(String strUrl, String strBody, IRhoSession oSession)throws Exception
	{
		Hashtable headers = new Hashtable();
		m_bIgnoreSuffixOnSim = false;
		m_bCancel = false;
    	
		NetResponse resp = doRequest/*Try*/("POST", strUrl, strBody, oSession, headers);
		if ( resp.isOK() )
		{
			resp.setCharData(resp.getCookies());
			LOG.INFO("pullCookies: " + resp.getCharData() );
		}
		
		return resp;
	}
	
	static String szMultipartPostfix = 
	    "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

	static String szMultipartContType = 
	    "multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5";

	void processMultipartItems( Vector/*Ptr<CMultipartItem*>&*/ arItems )throws Exception
	{
	    for( int i = 0; i < (int)arItems.size(); i++ )
	    {
	        MultipartItem oItem = (MultipartItem)arItems.elementAt(i); 

	        if ( oItem.m_strName.length() == 0 )
	            oItem.m_strName = "blob";

	        if ( oItem.m_strFileName.length() == 0 )
	        {
	            if ( oItem.m_strFilePath.length() > 0 )
	            {
	                FilePath oPath = new FilePath(oItem.m_strFilePath);
	                oItem.m_strFileName = oPath.getBaseName();
	            }
	            //else
	            //    oItem.m_strFileName = "doesnotmatter.txt";
	        }

	        oItem.m_strDataPrefix = i > 0 ? "\r\n" : "";
	        oItem.m_strDataPrefix += 
	            "------------A6174410D6AD474183FDE48F5662FCC5\r\n"+
	            "Content-Disposition: form-data; name=\"";
	        oItem.m_strDataPrefix += oItem.m_strName + "\"";
	        if (oItem.m_strFileName.length()>0)
	            oItem.m_strDataPrefix += "; filename=\"" + oItem.m_strFileName + "\"";
	        oItem.m_strDataPrefix += "\r\n";
	        if ( oItem.m_strContentType.length() > 0 )
	            oItem.m_strDataPrefix += "Content-Type: " + oItem.m_strContentType + "\r\n";

	        long nContentSize = 0;
	        if ( oItem.m_strFilePath.length() > 0 )
	        {
	        	SimpleFile file = null;
	    		try{
	    			file = RhoClassFactory.createFile();
	    			file.open(oItem.m_strFilePath, true, true);
	    			nContentSize = file.length();
	    			if ( !file.isOpened() ){
	    				LOG.ERROR("File not found: " + oItem.m_strFilePath);
	    				throw new RuntimeException("File not found:" + oItem.m_strFilePath);
	    			}
	    		}finally{
	    			if ( file != null )
	    				try{ file.close(); }catch(IOException e){}
	    		}
	        }
	        else
	            nContentSize = oItem.m_strBody.length();

	        if ( oItem.m_strContentType.length() > 0 )
	            oItem.m_strDataPrefix += "Content-Length: " + nContentSize + "\r\n";

	        oItem.m_strDataPrefix += "\r\n";
	    }

	}
	
    public NetResponse pushMultipartData(String strUrl, Vector/*Ptr<CMultipartItem*>&*/ arItems, IRhoSession oSession, Hashtable/*<String,String>**/ headers)throws Exception
    {
		String strRespBody = null;
		InputStream is = null;
		OutputStream os = null;
		int code  = -1;
		
		m_bCancel = false;
    	
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl, false);
			
			if ( oSession != null )
			{
				String strSession = oSession.getSession();
				if ( strSession != null && strSession.length() > 0 )
					m_connection.setRequestProperty("Cookie", strSession );
			}
			
			m_connection.setRequestProperty("Connection", "keep-alive");
			m_connection.setRequestProperty("content-type", szMultipartContType);
			writeHeaders(headers);
			m_connection.setRequestMethod(IHttpConnection.POST);
			
			//PUSH specific
			processMultipartItems( arItems );
			os = m_connection.openOutputStream();
	        //write all items
	        for( int i = 0; i < (int)arItems.size(); i++ )
	        {
	            MultipartItem oItem = (MultipartItem)arItems.elementAt(i); 
    			os.write(oItem.m_strDataPrefix.getBytes(), 0, oItem.m_strDataPrefix.length());

	            if ( oItem.m_strFilePath.length() > 0 )
	            {

		        	SimpleFile file = null;
		    		InputStream fis = null;
		        	
		    		try{
		    			file = RhoClassFactory.createFile();
		    			file.open(oItem.m_strFilePath, true, true);
		        		
		    			if ( !file.isOpened() ){
		    				LOG.ERROR("File not found: " + oItem.m_strFilePath);
		    				throw new RuntimeException("File not found:" + oItem.m_strFilePath);
		    			}
		    			
		    			fis = file.getInputStream();
		    			byte[]  byteBuffer = new byte[1024*4]; 
		    			int nRead = 0;
		        		do{
		        			nRead = fis.read(byteBuffer);	    			
		        			if ( nRead > 0 )
		        				os.write(byteBuffer, 0, nRead);
		        		}while( nRead > 0 );
		    		}finally{
						if (fis != null)
							try{ fis.close(); }catch(IOException e){}
		    			
		    			if ( file != null )
		    				try{ file.close(); }catch(IOException e){}
		    		}
	    			
	            }else
	            {
	    			os.write(oItem.m_strBody.getBytes(), 0, oItem.m_strBody.length());
	            }
	        }
			os.write(szMultipartPostfix.getBytes(), 0, szMultipartPostfix.length());
			//os.flush();
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
					strRespBody = readFully(is);
				
			}else
			{
				long len = m_connection.getLength();
				LOG.INFO("fetchRemoteData data size:" + len );
		
				strRespBody = readFully(is);
				
				LOG.INFO("fetchRemoteData data readFully.");
			}
			
			readHeaders(headers);			
		}finally{
			try{
				if ( is != null )
					is.close();
				if (os != null)
					os.close();
				
				closeConnection();
				
			}catch(IOException exc2){}
		}
		
		copyHashtable(m_OutHeaders, headers);
		return makeResponse(strRespBody, code );
    }
	
	long m_nMaxPacketSize = 0;
	int m_nCurDownloadSize = 0;
	boolean m_bFlushFileAfterWrite = false;
	public NetResponse pullFile( String strUrl, String strFileName, IRhoSession oSession, Hashtable headers )throws Exception
	{
		IRAFile file = null;
		NetResponse resp = null;
		
		m_nMaxPacketSize = RhoClassFactory.getNetworkAccess().getMaxPacketSize();
		m_bFlushFileAfterWrite = RhoConf.getInstance().getBool("use_persistent_storage");
		m_bCancel = false;
    	
		try{

	        if (!strFileName.startsWith("file:")) { 
            	try{
	            	strFileName = FilePath.join(RhoClassFactory.createFile().getDirPath(""), strFileName);
            	} catch (IOException x) { 
                 	LOG.ERROR("getDirPath failed.", x);
                    throw x;
                }              	
	        }
			
			file = RhoClassFactory.createFSRAFile();
			file.open(strFileName, "rw");
			file.seek(file.size());
			
			do{
				resp = pullFile1( strUrl, file, file.size(), oSession, headers );
			}while( !m_bCancel && (resp == null || resp.isOK()) && m_nCurDownloadSize > 0 && m_nMaxPacketSize > 0 );
			
		}finally{
			if ( file != null )
			{
				file.close();
				file = null;
			}
		}
		
		copyHashtable(m_OutHeaders, headers);
		
		return resp != null && !m_bCancel ? resp : makeResponse("", IHttpConnection.HTTP_INTERNAL_ERROR );
	}
	
	NetResponse pullFile1( String strUrl, IRAFile file, long nStartPos, IRhoSession oSession, Hashtable headers )throws Exception
	{
		String strRespBody = null;
		InputStream is = null;
		int code = -1;
		
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl, true);
			
			if (oSession!= null)
			{
				String strSession = oSession.getSession();
				if ( strSession != null && strSession.length() > 0 )
					m_connection.setRequestProperty("Cookie", strSession );
			}
			
			m_connection.setRequestProperty("Connection", "keep-alive");
			
			if ( nStartPos > 0 || m_nMaxPacketSize > 0 )
			{
				if ( m_nMaxPacketSize > 0 )
					m_connection.setRequestProperty("Range", "bytes="+nStartPos+"-" + (nStartPos + m_nMaxPacketSize-1));
				else
					m_connection.setRequestProperty("Range", "bytes="+nStartPos+"-");
			}
			writeHeaders(headers);
			
			m_connection.setRequestMethod(IHttpConnection.GET);
			
			code = m_connection.getResponseCode();
			
			LOG.INFO("getResponseCode : " + code);
			
			m_nCurDownloadSize = 0;
			
			if ( code == IHttpConnection.HTTP_RANGENOTSATISFY )
				code = IHttpConnection.HTTP_PARTIAL_CONTENT;
			else
			{
				if (code != IHttpConnection.HTTP_OK && code != IHttpConnection.HTTP_PARTIAL_CONTENT ) 
				{
					LOG.ERROR("Error retrieving data: " + code);
					if (code == IHttpConnection.HTTP_UNAUTHORIZED) 
						oSession.logout();
					
					if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
					{
						is = m_connection.openInputStream();
						strRespBody = readFully(is);
					}
				}else
				{
					int nRead = 0;
					
					is = m_connection.openInputStream();
					byte[]  byteBuffer = new byte[1024*20]; 

		    		do{
		    			nRead = /*bufferedReadByByte(m_byteBuffer, is);*/is.read(byteBuffer);
		    			if ( nRead > 0 )
		    			{
		    				file.write(byteBuffer, 0, nRead);
		    				
		    				if (m_bFlushFileAfterWrite)
		    					file.sync();

		    				m_nCurDownloadSize += nRead;
		    			}
		    		}while( !m_bCancel && nRead >= 0 );
		    		
		    		if ( code == IHttpConnection.HTTP_PARTIAL_CONTENT && isFinishDownload() )
		    			m_nCurDownloadSize = 0;
				}
				
			}

			readHeaders(headers);
		}finally
		{
			if ( is != null )
				try{ is.close(); }catch(IOException exc){}
			
			closeConnection();
		}
		
		return makeResponse(strRespBody != null ? strRespBody : "", code );
	}
	
	private boolean isFinishDownload()throws IOException
	{
		String strContRange = m_connection.getHeaderField("Content-Range");
		if ( strContRange != null )
		{
			int nMinus = strContRange.indexOf('-');
			if ( nMinus > 0 )
			{
				int nSep = strContRange.indexOf('/', nMinus);
				if ( nSep > 0 )
				{
					String strHigh = strContRange.substring(nMinus+1,nSep);
					String strTotal = strContRange.substring(nSep+1);
					
					if ( Integer.parseInt(strHigh) + 1 >= Integer.parseInt(strTotal) )
						return true;
				}
			}
		}
		
		return false;
	}
	
	public String resolveUrl(String url) throws Exception
    {
		if ( url == null || url.length() == 0 )
			return "";

		String _httpRoot = RhoClassFactory.getNetworkAccess().getHomeUrl();
		if(!_httpRoot.endsWith("/"))
			_httpRoot = _httpRoot + "/";
		url.replace('\\', '/');
		if ( !url.startsWith(_httpRoot) )
			url = FilePath.join(_httpRoot, url);
		
		return url;
    }

	public void cancel()
    {
		m_bCancel = true;
		closeConnection();
    }

	/*static{
		TEST();
	}
	public static void TEST()
	{
		//ParsedCookie cookie = new ParsedCookie();
		String strClientCookie = "";
		strClientCookie = URI.parseCookie("auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT, auth_token=887b2ffd30a7b97be9a0986d7746a934421eec7d; path=/; expires=Sat, 24 Oct 2009 20:56:55 GMT, rhosync_session=BAh7BzoMdXNlcl9pZGkIIgpmbGFzaElDOidBY3Rpb25Db250cm9sbGVyOjpGbGFzaDo6Rmxhc2hIYXNoewAGOgpAdXNlZHsA--f9b67d99397fc534107fb3b7483ccdae23b4a761; path=/; expires=Sun, 10 Oct 2010 19:10:58 GMT; HttpOnly");
		strClientCookie = URI.parseCookie("auth_token=; path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT");
		strClientCookie = URI.parseCookie("rhosync_session=BAh7CToNcGFzc3dvcmQiFTiMYru1W11zuoAlN%2FPtgjc6CmxvZ2luIhU4jGK7tVtdc7qAJTfz7YI3Ogx1c2VyX2lkaQYiCmZsYXNoSUM6J0FjdGlvbkNvbnRyb2xsZXI6OkZsYXNoOjpGbGFzaEhhc2h7AAY6CkB1c2VkewA%3D--a7829a70171203d72cd4e83d07b18e8fcf5e2f78; path=/; expires=Thu, 02 Sep 2010 23:51:31 GMT; HttpOnly");
	}*/
	
	private static String makeClientCookie(Hashtable headers)
			throws IOException 
	{
		if ( headers == null )
			return "";
		
		//ParsedCookie cookie = new ParsedCookie();
		String strRes = "";
    	Enumeration valsHeaders = headers.elements();
    	Enumeration keysHeaders = headers.keys();
		while (valsHeaders.hasMoreElements()) 
		{
			String strName = (String)keysHeaders.nextElement();
			String strValue = (String)valsHeaders.nextElement();

			if (strName.equalsIgnoreCase("Set-Cookie")) 
			{
				LOG.INFO("Set-Cookie: " + strValue);
				
				strRes += URI.parseCookie(strValue);
			}
		}

		return strRes;
	}

	private final String readFully(InputStream in) throws Exception 
	{
		String strRes = "";
		byte[]  byteBuffer = new byte[1024*4];
		
		int nRead = 0;
		do{
			nRead = in.read(byteBuffer);
			if (nRead>0)
			{
				String strTemp = new String(byteBuffer,0,nRead);
				strRes += strTemp;
			}
		}while( nRead > 0 );
		
		return strRes;
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

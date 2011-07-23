package com.rho.net;

import java.util.HashMap;
import java.util.Vector;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.sync.SyncEngine;

public class NetRequest
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Net");

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
	}

	private boolean m_bIgnoreSuffixOnSim;
	private boolean m_bCancel;

	public void cancel() {
		// TODO Auto-generated method stub
		
	}

	public NetResponse pullData(Object clientCreateUrl, SyncEngine syncEngine) {
		// TODO Auto-generated method stub
		return null;
	}

	public NetResponse pullCookies(String strUrl, String strBody, IRhoSession oSession)throws Exception
	{
		HashMap headers = new HashMap();
		m_bIgnoreSuffixOnSim = false;
		m_bCancel = false;
    	
		NetResponse resp = doRequest("POST", strUrl, strBody, oSession, headers);
		if ( resp.isOK() )
		{
			String strCookie = resp.getCookies();
			if ( strCookie == null || strCookie.length() == 0 )
				strCookie = "rho_empty";
			
			resp.setCharData(strCookie);
			LOG.INFO("pullCookies: " + resp.getCharData() );
		}
		
		return resp;
	}

	public native NetResponse doRequestJS(String strMethod, String strUrl, String strBody, IRhoSession oSession, HashMap headers )
	/*-{
    	var result = $wnd.jQuery.ajax({
  			async: false,
  			type: strMethod,
  			url: strUrl,
  			data: strBody,
  			cache: false,
			beforeSend: function(xhrObj, settings){
                xhrObj.setRequestHeader("Content-Type","text/plain");
                // xhrObj.setRequestHeader("Accept","application/json");
        	}
        });
 		var netResponse = {
 			body: result.responseText,
 			statusCode: result.status,
 			statusText: result.statusText
 		};
		return netResponse;
    }-*/;

	public NetResponse doRequest(String strMethod, String strUrl, String strBody, IRhoSession oSession, HashMap headers ) throws Exception
    {
		NetResponse resp = doRequestJS(strMethod, strUrl, strBody, oSession, headers);
		LOG.INFO("connection done");
		LOG.INFO("getResponseCode : " + resp.getRespCodeText());

		int code = resp.getRespCode();

		if ( code >= 400 ) 
		{
			LOG.ERROR("Error retrieving data: " + code);
			if (code == IHttpConnection.HTTP_UNAUTHORIZED && oSession != null)
			{
				LOG.ERROR("Unauthorize error.Client will be logged out");
				oSession.logout();
			}
			LOG.TRACE("Response body: " + resp.getCharData() );
		}else
		{
			LOG.TRACE("Response body: " + resp.getCharData() );
			LOG.INFO("fetchRemoteData data readFully.");
		}

		return resp;

		/*
		String strRespBody = null;
		InputStream is = null;
		OutputStream os = null;
		int code = -1;
		
		m_bCancel = false;
		try{
			closeConnection();
			m_connection = RhoClassFactory.getNetworkAccess().connect(strUrl, m_bIgnoreSuffixOnSim);
			LOG.INFO("connection done");
			
			handleCookie(oSession);
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
				//m_connection.setRequestMethod(IHttpConnection.POST);
				m_connection.setRequestMethod(strMethod);
				
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

			readHeaders(headers);
			copyHashtable(m_OutHeaders, headers);
			
			if ( code >= 400 ) 
			{
				LOG.ERROR("Error retrieving data: " + code);
				if (code == IHttpConnection.HTTP_UNAUTHORIZED && oSession != null)
				{
					LOG.ERROR("Unauthorize error.Client will be logged out");
					oSession.logout();
				}
				
				//if ( code != IHttpConnection.HTTP_INTERNAL_ERROR )
				{
					strRespBody = readFully(is, getResponseEncoding());
					LOG.TRACE("Response body: " + strRespBody );
				}
			}else
			{
				long len = m_connection.getLength();
				LOG.INFO("fetchRemoteData data size:" + len );
		
				strRespBody = readFully(is, getResponseEncoding());
				
				LOG.INFO("fetchRemoteData data readFully.");
			}

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
  		*/
      }
	
	public NetResponse pullFile(String strHsqlScriptUrl, String fScriptName,
			SyncEngine syncEngine, Object object) {
		// TODO Auto-generated method stub
		return null;
	}

	public NetResponse pushData(Object clientChangesUrl, String strBody,
			SyncEngine sync) {
		// TODO Auto-generated method stub
		return null;
	}

	public NetResponse pushMultipartData(Object clientChangesUrl,
			Vector m_arMultipartItems, SyncEngine sync, Object object) {
		// TODO Auto-generated method stub
		return null;
	}

	public Object resolveUrl(String string) {
		// TODO Auto-generated method stub
		return null;
	};
	
/*
	public boolean isCancelled(){ return false; }
	
	//TODO: use sslVerifyPeer
    public boolean sslVerifyPeer() {return false;}
    public void sslVerifyPeer(boolean mode) { }
	
	public NetResponse pullData(String strUrl, IRhoSession oSession ) throws Exception
    {
		return null;
    }

	public void setIgnoreSuffixOnSim(boolean bset)
	{
	}

	private void writeHeaders(Hashtable headers) throws Exception
	{
	}

	private void readHeaders(Hashtable headers) throws Exception
	{
	}
	
	public static void copyHashtable(Hashtable from, Hashtable to)
	{
	}
	
	private String getResponseEncoding() throws Exception
	{
	}
	
	void handleCookie(IRhoSession oSession) throws Exception
	{
	}
	
	public NetResponse doRequest(String strMethod, String strUrl, String strBody, IRhoSession oSession, Hashtable headers ) throws Exception
    {
		return null;
    }
	
	private NetResponse makeResponse(String strRespBody, int nErrorCode)throws Exception
	{
		return null;
	}
	
	public NetResponse pushData(String strUrl, String strBody, IRhoSession oSession)throws Exception
    {
		return null;
    }

    public NetResponse pushMultipartData(String strUrl, MultipartItem oItem, IRhoSession oSession, Hashtable pHeaders)throws Exception
    {
        return null;
    }
	
	public NetResponse pullCookies(String strUrl, String strBody, IRhoSession oSession)throws Exception
	{
		return null;
	}
	
	void processMultipartItems( Vector arItems )throws Exception
	{
	}
	
    public NetResponse pushMultipartData(String strUrl, Vector arItems, IRhoSession oSession, Hashtable headers)throws Exception
    {
		return null;
    }
	
	public NetResponse pullFile( String strUrl, String strFileName, IRhoSession oSession, Hashtable headers )throws Exception
	{
		return null;
	}
	
	NetResponse pullFile1( String strUrl, IRAFile file, long nStartPos, IRhoSession oSession, Hashtable headers )throws Exception
	{
		return null;
	}
	
	private boolean isFinishDownload()throws IOException
	{
		return false;
	}
	
	public String resolveUrl(String strUrl) throws Exception
    {
	    return "";
    }

	public void cancel()
    {
    }

	public static final String readFully(InputStream in, String strContType) throws Exception 
	{
		return "";
	}
	
	public void closeConnection(){
	}
*/	
}

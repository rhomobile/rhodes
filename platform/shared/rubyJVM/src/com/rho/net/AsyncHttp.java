package com.rho.net;

import com.rho.*;
import com.rho.rjson.RJSONTokener;
import com.rho.sync.SyncThread;

import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.*;
import com.rho.net.NetRequest.MultipartItem;
import com.rho.ThreadQueue;

import java.io.IOException;

public class AsyncHttp extends ThreadQueue
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("AsyncHttp");
	private static RhodesApp RHODESAPP(){ return RhodesApp.getInstance(); }
	
    static AsyncHttp m_pInstance;
    HttpCommand m_pCurCmd;

    static AsyncHttp Create()
    {
	    if ( m_pInstance != null) 
	        return m_pInstance;
	
	    m_pInstance = new AsyncHttp(new RhoClassFactory());
	    return m_pInstance;
    }
    
    static void Destroy()
    {
    	if ( m_pInstance != null )
    	{
    		m_pInstance.cancelRequest("*", true);
	        LOG.INFO("Thread shutdown");
	        
	        m_pInstance = null;
    	}
    }
    
    static AsyncHttp getInstance(){ return m_pInstance; }
    
    AsyncHttp(RhoClassFactory factory)
    {
    	super(factory);
        ThreadQueue.setLogCategory(LOG.getLogCategory());

        setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);

        m_pCurCmd = null;
    }
    
    void cancelRequest(String szCallback, boolean bWait)
    {
        if (szCallback == null || szCallback.length() == 0 )
        {
            LOG.INFO("Cancel callback should not be empty. Use * for cancel all");
            return;
        }

        if ( m_pCurCmd != null )
            m_pCurCmd.cancel();

        if ( bWait )
            stop(-1);

        //TODO: find command by callback and cancel it if current, remove if it is still in queue
    }
    
    public void addQueueCommand(IQueueCommand pCmd)
    {
        if ( ((HttpCommand)pCmd).m_strCallback.length()==0)
        	processCommand(pCmd);
        else
        {
            super.addQueueCommand(pCmd);
            start(epLow);
        }
    }
    
    public void processCommand(IQueueCommand pCmd)
    {
        m_pCurCmd = (HttpCommand)pCmd;
        m_pCurCmd.execute();
        m_pCurCmd = null;
    }
    
	public final static int  hcNone = 0, hcGet = 1, hcPost=2, hcDownload=3, hcUpload =4;
    
    private static class HttpCommand implements IQueueCommand
    {
	    int m_eCmd;
        String m_strCallback, m_strCallbackParams;
        Hashtable/*<String,String>*/ m_mapHeaders = new Hashtable();

        private NetRequest m_pNetRequest;
        String m_strResBody;
        private RubyValue m_valBody;
        
        RhoParams    m_params;

        HttpCommand(String strCmd, RubyValue p)
        {
        	m_params = new RhoParams(p); 
            m_eCmd = translateCommand(strCmd);
            m_strCallback = m_params.getString("callback");
            m_strCallbackParams = m_params.getString("callback_param");

            m_mapHeaders = m_params.getHash("headers");

            m_pNetRequest = AsyncHttp.getInstance().getFactory().createNetRequest();
            m_pNetRequest.sslVerifyPeer(m_params.getBool("ssl_verify_peer"));
        }

        int translateCommand(String strCmd)
        {
            if ( strCmd.compareTo("GET") == 0 )
                return hcGet;
            else if ( strCmd.compareTo("POST") == 0 )
                return hcPost;
            else if ( strCmd.compareTo("Download") == 0 )
                return hcDownload;
            else if ( strCmd.compareTo("Upload") == 0 )
                return hcUpload;

            return hcNone;
        }

    	void processResponse(NetResponse resp )
    	{
    	    if (resp.isOK() && m_mapHeaders != null)
    	    {
    	    	String strContType = (String)m_mapHeaders.get("content-type");
    	    	if ( strContType != null && strContType.indexOf("application/json") >=0 )
    	    	{
    	    		RJSONTokener json = new RJSONTokener(resp.getCharData());
    	    		
    	    		try{
    	    			m_valBody = json.nextRValue();
    	    			
    	    			return;
    	    		}catch(Exception exc)
    	    		{
    	    			LOG.ERROR("Incorrect json body.", exc);
    	    		}
    	    	}
    	    }

    	    m_valBody = RhoRuby.create_string(resp.getCharData());
    	}
        
        void execute()
        {
            NetResponse resp = null;
     	    try
     	    {
	            switch( m_eCmd )
	            {
	            case hcGet:
	            	m_pNetRequest.setIgnoreSuffixOnSim(false);
	                resp = m_pNetRequest.doRequest( m_params.getString("http_command", "GET"), 
	                    m_params.getString("url"), m_params.getString("body"), null, m_mapHeaders);
	                break;
	            case hcPost:
	            	m_pNetRequest.setIgnoreSuffixOnSim(false);
	                resp = m_pNetRequest.doRequest(m_params.getString("http_command", "POST"), 
	                    m_params.getString("url"), m_params.getString("body"), null, m_mapHeaders);
	                break;
	
	            case hcDownload:
	                resp = m_pNetRequest.pullFile(m_params.getString("url"), m_params.getString("filename"), null, m_mapHeaders);
	                break;
	
	            case hcUpload:
	                {
	                    Vector/*Ptr<net::CMultipartItem*>*/ arMultipartItems = new Vector();
	
	                    RhoParamArray arParams = new RhoParamArray( m_params, "multipart");
	                    if ( arParams.size() > 0 )
	                    {
	                        for( int i = 0; i < arParams.size(); i++)
	                        {
	                            RhoParams oItem = arParams.getItem(i);
	
	                            MultipartItem pItem = new MultipartItem();
	                            String strBody = oItem.getString("body");
	                            if ( strBody.length() > 0 )
	                            {
	                                pItem.m_strBody = strBody;
	                                pItem.m_strContentType = oItem.getString("content_type", "application/x-www-form-urlencoded");
	                            }
	                            else
	                            {
	                                pItem.m_strFilePath = oItem.getString("filename");
	                                pItem.m_strContentType = oItem.getString("content_type", "application/octet-stream");
	                            }
	
	                            pItem.m_strName = oItem.getString("name");
	                            pItem.m_strFileName = oItem.getString("filename_base");
	                            arMultipartItems.addElement(pItem);
	                        }
	                    }else
	                    {
	                        MultipartItem pItem = new MultipartItem();
	                        pItem.m_strFilePath = m_params.getString("filename");
	                        pItem.m_strContentType = m_params.getString("file_content_type", "application/octet-stream");
	                        pItem.m_strName = m_params.getString("name");
	                        pItem.m_strFileName = m_params.getString("filename_base");
	                        arMultipartItems.addElement(pItem);
	
	                        String strBody = m_params.getString("body");
	                        if ( strBody.length() > 0 )
	                        {
	                            MultipartItem pItem2 = new MultipartItem();
	                            pItem2.m_strBody = strBody;
	                            pItem2.m_strContentType = (String)m_mapHeaders.get("content-type");
	                            arMultipartItems.addElement(pItem2);
	                        }
	                    }
	
	                    resp = m_pNetRequest.pushMultipartData( m_params.getString("url"), arMultipartItems, null, m_mapHeaders );
	                    break;
	                }
	            }
	
	            if ( !m_pNetRequest.isCancelled())
	            {
				    processResponse(resp);
	                callNotify(resp,0);
	            }
    	    }catch(IOException exc)
    	    {
    	    	LOG.ERROR("command failed: " + m_eCmd + "url: " + m_params.getString("url"), exc);
    	    	callNotify(null, RhoRuby.ERR_NETWORK);
    	    }catch(Exception exc)
    	    {
    	    	LOG.ERROR("command crashed: " + m_eCmd + "url: " + m_params.getString("url"), exc);
    	    	callNotify(null, RhoRuby.ERR_RUNTIME);
    	    }
        }
        
        void cancel()
        {
            if (m_pNetRequest!=null && !m_pNetRequest.isCancelled() )
                m_pNetRequest.cancel();
        }

        void callNotify(NetResponse resp, int nError )
        {
            m_strResBody = "rho_callback=1";
            m_strResBody += "&status=";
            if ( nError > 0 )
            {
            	m_strResBody += "error&error_code=" + nError;
            }else
            {
                if ( resp.isOK() )
            	    m_strResBody += "ok";
                else
                {
            	    m_strResBody += "error&error_code=";
                    m_strResBody += RhoRuby.getErrorFromResponse(resp);
                    //if ( resp.isResponseRecieved())
        	            m_strResBody += "&http_error=" + resp.getRespCode();
                }

                String cookies = resp.getCookies();
                if (cookies.length()>0)
                    m_strResBody += "&cookies=" + URI.urlEncode(cookies);

                String strHeaders = makeHeadersString();
                if (strHeaders.length() > 0 )
                    m_strResBody += "&" + strHeaders;

                m_strResBody += "&" + RHODESAPP().addCallbackObject(m_valBody, "body");
            }

            if ( m_strCallbackParams.length() > 0 )
                m_strResBody += "&" + m_strCallbackParams;

            if ( m_strCallback.length() > 0 )
            {
    		    try{
	                String strFullUrl = m_pNetRequest.resolveUrl(m_strCallback);
	                NetResponse resp1 = m_pNetRequest.pushData( strFullUrl, m_strResBody, null );
    		        if ( !resp1.isOK() )
    		            LOG.ERROR("AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData() );
    	        }catch(Exception exc)
    	        {
    	        	LOG.ERROR("Async http callback failed.", exc);
    	        }
            }
        }
        
        RubyValue getRetValue()
        {
    	    if ( m_strCallback.length() == 0 )
    	        return RhoRuby.create_string(m_strResBody); 

    	    return RubyConstant.QNIL;
        }

	    public boolean equals(IQueueCommand cmd){ return false; }

        public String toString()
        {
            switch(m_eCmd)
            {
            case hcGet:
                return "GET";
            case hcPost:
                return "POST";
            case hcDownload:
                return "Download";
            case hcUpload:
                return "Upload";
            }
            return "Unknown";
        }

        String makeHeadersString()
        {
    	    String strRes = "";

        	Enumeration valsHeaders = m_mapHeaders.elements();
        	Enumeration keysHeaders = m_mapHeaders.keys();
    		while (valsHeaders.hasMoreElements()) 
    		{
    	        if ( strRes.length() > 0)
    	            strRes += "&";

    	        strRes += "headers[";
    	        strRes += URI.urlEncode((String)keysHeaders.nextElement());
    	        strRes += "]=";
    	        strRes += URI.urlEncode((String)valsHeaders.nextElement());
    	    }

    	    return strRes;
        }

    };
	
	public static void initMethods(RubyModule klass) 
	{
		klass.getSingletonClass().defineMethod("do_request", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block )
			{
				try {
				    AsyncHttp.Create();
				    
				    String command = arg1.toStr();
				    AsyncHttp.HttpCommand pHttp = new AsyncHttp.HttpCommand( command, arg2 );
				    AsyncHttp.getInstance().addQueueCommand(pHttp);
				    return pHttp.getRetValue();
				} catch(Exception e) {
					LOG.ERROR("do_request failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.getSingletonClass().defineMethod("cancel", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					String cancel_callback = arg.toStr();
				    if ( AsyncHttp.getInstance() != null )
				        AsyncHttp.getInstance().cancelRequest(cancel_callback, false);
					
				} catch(Exception e) {
					LOG.ERROR("cancel failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		

	}
	
}

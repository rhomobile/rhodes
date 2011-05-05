using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.IO;
using System.Collections.Generic;
using rho.common;
using fastJSON;
using IronRuby.Runtime;
using IronRuby.Builtins;

namespace rho.net
{
    public class CAsyncHttp : CThreadQueue 
    {
        private RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("AsyncHttp");
        public RhoLogger getLog() { return LOG;  }
	    private static CRhodesApp RHODESAPP(){ return CRhodesApp.Instance; }
	    
        static CAsyncHttp m_pInstance;
        private bool m_bInternal = false;

        public static CAsyncHttp Create()
        {
	        if ( m_pInstance != null) 
	           return m_pInstance;
	
	        m_pInstance = new CAsyncHttp();
	        return m_pInstance;
        }

        static public void Destroy()
        {
            if (m_pInstance != null)
            {
                m_pInstance.stop(2);

                m_pInstance = null;
            }
        }

        public static CAsyncHttp getInstance(){ return m_pInstance; }
    
        public CAsyncHttp(bool bInternal = false)
        {
            m_bInternal = bInternal;
            if (!m_bInternal)
            {
                base.setLogCategory(LOG.getLogCategory());
            }
            else
            {
                LOG = new RhoEmptyLogger();
                base.setLogCategory(null);
            }

            setPollInterval(QUEUE_POLL_INTERVAL_INFINITE);
        }

        public void cancelRequest(String szCallback)
        {
            if (szCallback == null || szCallback.length() == 0 )
            {
                LOG.INFO("Cancel callback should not be empty. Use * for cancel all");
                return;
            }

            lock(getCommandLock())
            {
	            HttpCommand pCmd = (HttpCommand)getCurCommand();
	
	            if ( pCmd != null && ( szCallback.compareTo("*") == 0 || pCmd.m_strCallback.compareTo(szCallback) == 0) )
	                pCmd.cancel();
	
	            if ( szCallback.compareTo("*") == 0 )
	                getCommands().Clear();
	            else
	            {
	                for (int i = getCommands().size()-1; i >= 0; i--)
	                {
	                    HttpCommand pCmd1 = (HttpCommand)getCommands().get(i);
	
	                    if ( pCmd1 != null && pCmd1.m_strCallback.compareTo(szCallback) == 0 )
    	                    getCommands().RemoveAt(i);
	                }
	
	            }
            }   
        }

        public MutableString addHttpCommand(HttpCommand pCmd)
        {
            pCmd.setLog(LOG);
            pCmd.setInternal(m_bInternal);
            if (!m_bInternal && pCmd.m_strCallback.length() == 0)
            {
                processCommandBase(pCmd);
                return pCmd.getRetValue();
            }
            else
            {
                addQueueCommand(pCmd);
                start(epLow);

                return pCmd.getRetValue();
            }
        }

        public override void processCommand(IQueueCommand pCmd)
        {
            ((HttpCommand)pCmd).execute();
        }

	    public const int  hcNone = 0, hcGet = 1, hcPost=2, hcDownload=3, hcUpload =4;

        public class HttpCommand : IQueueCommand
        {
            private RhoLogger LOG;
            public void setLog(RhoLogger log) { LOG = log; }

	        int m_eCmd;
            public String m_strCallback, m_strCallbackParams;
            Hashtable<String, String> m_mapHeaders = new Hashtable<String, String>();

            private NetRequest m_pNetRequest;
            String m_strResBody;
            private MutableString m_valBody;    
        
            RhoParams    m_params;
            bool m_bInternal = false;
            public void setInternal(bool b) { m_bInternal = b;  }

            public HttpCommand(String strCmd, RhoParams p)
            {
                m_params = new RhoParams(p); 
                m_eCmd = translateCommand(strCmd);
                m_strCallback = m_params.getString("callback");
                m_strCallbackParams = m_params.getString("callback_param");

                m_mapHeaders = m_params.getHash("headers");

                m_pNetRequest = RhoClassFactory.createNetRequest();
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
    	    	    String strContType = null;
                    m_mapHeaders.TryGetValue("content-type", out strContType);
    	    	    if ( strContType != null && strContType.indexOf("application/json") >=0 )
    	    	    {   
    	    		   ///// RJSONTokener json = new RJSONTokener(resp.getCharData());
    	    		    try{
    	    			  /////  m_valBody = json.nextRValue();
                            fastJSON.RJSONTokener.JsonDecode(resp.getCharData());
    	    			    return;
    	    		    }catch(Exception exc)
    	    		    {
    	    			    LOG.ERROR("Incorrect json body.", exc);
    	    		    }
    	    	    }
    	        }

    	        m_valBody = CRhoRuby.create_string(resp.getCharData());
    	    }
        
            public void execute()
            {
                NetResponse resp = null;
     	        try
     	        {
	                switch( m_eCmd )
	                {
	                    case hcGet:
	            	        ///m_pNetRequest.setIgnoreSuffixOnSim(false);
	                        resp = m_pNetRequest.doRequest( m_params.getString("http_command", "GET"), 
	                        m_params.getString("url"), m_params.getString("body"), null, m_mapHeaders);
	                        break;
	                    case hcPost:
	            	        ///m_pNetRequest.setIgnoreSuffixOnSim(false);
	                        resp = m_pNetRequest.doRequest(m_params.getString("http_command", "POST"), 
	                        m_params.getString("url"), m_params.getString("body"), null, m_mapHeaders);
	                        break;
	
	                    case hcDownload:
	                        resp = m_pNetRequest.pullFile(m_params.getString("url"), m_params.getString("filename"), null, m_mapHeaders);
	                        break;  
	
	            case hcUpload:
	                {
                        Vector<NetRequest.MultipartItem>/*Ptr<net::CMultipartItem*>*/ arMultipartItems = new Vector<NetRequest.MultipartItem>();
	
	                    RhoParamArray arParams = new RhoParamArray( m_params, "multipart");
	                    if ( arParams.size() > 0 )
	                    {
	                        for( int i = 0; i < arParams.size(); i++)
	                        {
	                            RhoParams oItem = arParams.getItem(i);

                                NetRequest.MultipartItem pItem = new NetRequest.MultipartItem();
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
                            NetRequest.MultipartItem pItem = new NetRequest.MultipartItem();
	                        pItem.m_strFilePath = m_params.getString("filename");
	                        pItem.m_strContentType = m_params.getString("file_content_type", "application/octet-stream");
	                        pItem.m_strName = m_params.getString("name");
	                        pItem.m_strFileName = m_params.getString("filename_base");
	                        arMultipartItems.addElement(pItem);
	
	                        String strBody = m_params.getString("body");
	                        if ( strBody.length() > 0 )
	                        {
                                NetRequest.MultipartItem pItem2 = new NetRequest.MultipartItem();
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
    	    	callNotify(null, RhoAppAdapter.ERR_NETWORK);
    	    }catch(Exception exc)
    	    {
    	    	LOG.ERROR("command crashed: " + m_eCmd + "url: " + m_params.getString("url"), exc);
    	    	callNotify(null, RhoAppAdapter.ERR_RUNTIME);
    	    }
        }
        
        public void cancel()
        {
            if (m_pNetRequest!=null )
                m_pNetRequest.cancel();
        }

        void callNotify(NetResponse resp, int nError )
        {
            if (m_bInternal)
                return;

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
                    m_strResBody += RhoAppAdapter.getErrorFromResponse(resp);
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
        
        public MutableString getRetValue()
        {
            if (m_strCallback.length() == 0)
            {
             if(m_strResBody == null)
                 return CRhoRuby.create_string("");
             else
                return CRhoRuby.create_string(m_strResBody);
            }

    	    return null;
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

            foreach (KeyValuePair<String, String> kvp in m_mapHeaders)
            {
                if (strRes.length() > 0)
                    strRes += "&";

                strRes += "headers[";
                strRes += URI.urlEncode(kvp.Key.ToString());
                strRes += "]=";
                strRes += URI.urlEncode(kvp.Value.ToString());
            }

    	    return strRes;
        }

    }
    }
}

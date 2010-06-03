package com.rho.net;

import com.rho.*;
import com.rho.rjson.RJSONTokener;

import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.*;
import com.rho.net.NetRequest.MultipartItem;

import java.io.IOException;

public class AsyncHttp extends RhoThread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("AsyncHttp");

	private static Object m_mxInstances = new Object();
	private static Vector/*<CAsyncHttp*>*/ m_arInstances = new Vector();
	private RhoClassFactory m_ptrFactory;
	private Object m_mxRequest = new Object();
	
	private NetRequest m_pNetRequest;
	private NetResponse m_pNetResponse;
	private Hashtable/*<String,String>*/ m_mapHeaders = new Hashtable();

	private String m_strUrl, m_strBody, m_strCallback, m_strCallbackParams;
	private String m_strFilePath;
    private String m_strResBody = "";
    boolean m_bFinished = false;
    
	private RubyValue m_valBody;
	public final static int  hcGet = 0, hcPost=1, hcDownload=2, hcUpload =3;
	private int m_eCmd;
	
	private RhodesApp RHODESAPP(){ return RhodesApp.getInstance(); }
	
	AsyncHttp(RhoClassFactory factory, int eCmd,
		    String url, RubyValue headers, String body, String filepath, String callback, String callback_params)
	{
		super(factory);
		
	    m_ptrFactory = factory;
	    m_strUrl = url != null ? url : "";
	    m_strBody = body != null ? body : "";
	    m_strFilePath = filepath != null ? filepath : "";
	    m_strCallback = callback != null ? callback : "";
	    m_strCallbackParams = callback_params != null ? callback_params : "";
	    m_eCmd = eCmd;

	    m_mapHeaders = RhoRuby.enum_strhash(headers);

	    addNewObject(this);	    

	    if (m_strCallback.length()==0)
	        run();
	    else
	        start(epLow);
	}

	void cancel(boolean bWait)
	{
		synchronized(m_mxRequest)
		{		
		    if (m_pNetRequest!=null && !m_pNetRequest.isCancelled())
		        m_pNetRequest.cancel();
		}
		
		if ( bWait )
			stop(10000);
	    //delete this;
	}

	static void addNewObject(AsyncHttp pObj)
	{
	    synchronized(m_mxInstances)
	    {
	        while(true)
	        {
	            int nToDelete = -1;
	            for (int i = 0; i < (int)m_arInstances.size(); i++ )
	            {
	                if ( ((AsyncHttp)m_arInstances.elementAt(i)).m_bFinished )
	                {
	                    nToDelete = i;
	                    break;
	                }
	            }

	            if (nToDelete==-1)
	                break;

	            m_arInstances.removeElementAt(nToDelete);
	        }
	    	
	        m_arInstances.addElement(pObj);
	    }
	}
	
	static void cancelRequest(String szCallback, boolean bWait)
	{
	    if (szCallback == null|| szCallback.length() ==0 )
	    {
	        LOG.INFO("Cancel callback should not be empty. Use * for cancel all");
	        return;
	    }

		synchronized(m_mxInstances)
	    {		
	        if ( szCallback.compareTo("*") ==0 )
	        {
	            for (int i = 0; i < (int)m_arInstances.size(); i++ )
	                ((AsyncHttp)m_arInstances.elementAt(i)).cancel(bWait);
	        }else
	        {
	            for (int i = 0; i < (int)m_arInstances.size(); i++ )
	            {
	                if ( ((AsyncHttp)m_arInstances.elementAt(i)).m_strCallback.compareTo(szCallback) == 0 )
	                	((AsyncHttp)m_arInstances.elementAt(i)).cancel(bWait);    
	            }
	        }
	    }
	}

	public void run()
	{
		LOG.INFO("RhoHttp thread start.");

		synchronized(m_mxRequest)
		{		
			m_pNetRequest = m_ptrFactory.createNetRequest();
		}
		
	    try{
		    switch( m_eCmd )
		    {
		    case hcGet:
		    	m_pNetRequest.setIgnoreSuffixOnSim(false);
		        m_pNetResponse = m_pNetRequest.doRequest("GET", m_strUrl, m_strBody, null, m_mapHeaders);
		        break;
		    case hcPost:
		    	m_pNetRequest.setIgnoreSuffixOnSim(false);
		        m_pNetResponse = m_pNetRequest.doRequest("POST", m_strUrl, m_strBody, null, m_mapHeaders);
		        break;
	
		    case hcDownload:
		        m_pNetResponse = m_pNetRequest.pullFile(m_strUrl, m_strFilePath, null, m_mapHeaders);
		        break;
	
		    case hcUpload:
		    	{
		            Vector/*Ptr<net::CMultipartItem*>*/ arMultipartItems = new Vector();

		            MultipartItem oItem = new MultipartItem();
		            oItem.m_strFilePath = m_strFilePath;
		            oItem.m_strContentType = "application/octet-stream";
		            arMultipartItems.addElement(oItem);

		            if ( m_strBody.length() > 0 )
		            {
			            MultipartItem oItem2 = new MultipartItem();
			            oItem2.m_strBody = m_strBody;
			            oItem2.m_strContentType = (String)m_mapHeaders.get("content-type");
			            arMultipartItems.addElement(oItem2);
		            }
		    		
		            m_pNetResponse = m_pNetRequest.pushMultipartData( m_strUrl, arMultipartItems, null, m_mapHeaders );
			        break;
		    	}
		    }
		    
		    if ( !m_pNetRequest.isCancelled() )
		    {
			    processResponse(m_pNetResponse);
		        callNotify(m_pNetResponse, 0);
		    }
	    }catch(IOException exc)
	    {
	    	LOG.ERROR("command failed: " + m_eCmd + "url: " + m_strUrl, exc);
	    	callNotify(null, RhoRuby.ERR_NETWORK);
	    }catch(Exception exc)
	    {
	    	LOG.ERROR("command crashed: " + m_eCmd + "url: " + m_strUrl, exc);
	    	callNotify(null, RhoRuby.ERR_RUNTIME);
	    }finally
	    {
			LOG.INFO("RhoHttp thread end.");
			m_bFinished = true;			
	    }
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

	RubyValue getRetValue()
	{
	    if ( m_strCallback.length() == 0 )
	        return RhoRuby.create_string(m_strResBody); 

	    return RubyConstant.QNIL;
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
	        if ( strHeaders != null && strHeaders.length() > 0 )
	        	m_strResBody += "&" + makeHeadersString();
	        
	        m_strResBody += "&" + RHODESAPP().addCallbackObject(m_valBody, "body");
	    }
	    
	    if ( m_strCallbackParams.length() > 0 )
	    	m_strResBody += "&" + m_strCallbackParams;
	    	

	    if ( m_strCallback.length() > 0 )
	    {
		    try{
		        NetRequest pNetRequest = m_ptrFactory.createNetRequest();
	
		        String strFullUrl = pNetRequest.resolveUrl(m_strCallback);
		        NetResponse resp1 = pNetRequest.pushData( strFullUrl, m_strResBody, null );
		        if ( !resp1.isOK() )
		            LOG.ERROR("AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData() );
	        }catch(Exception exc)
	        {
	        	LOG.ERROR("Async http callback failed.", exc);
	        }
	    }
	}
	
	public static void initMethods(RubyModule klass) {
		klass.getSingletonClass().defineMethod("do_get", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 5 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_get: wrong number of arguments ( " + args.size() + " for " + 5 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String callback = args.get(2).toStr();
					String callback_params = args.get(3).toStr();
					boolean ssl_verify_peer = args.get(4).toInt() != 0 ? true : false;
					AsyncHttp pHttp = new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcGet, url, args.get(1), null, null, callback, callback_params );
					return pHttp.getRetValue();
				} catch(Exception e) {
					LOG.ERROR("do_get failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.getSingletonClass().defineMethod("do_post", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 6 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_post: wrong number of arguments ( " + args.size() + " for " + 6 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String body = args.get(2).toStr();
					String callback = args.get(3).toStr();
					String callback_params = args.get(4).toStr();
					boolean ssl_verify_peer = args.get(5).toInt() != 0 ? true : false;
					AsyncHttp pHttp = new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcPost, url, args.get(1), body, null, callback, callback_params );
					return pHttp.getRetValue();
				} catch(Exception e) {
					LOG.ERROR("do_post failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});		

		klass.getSingletonClass().defineMethod("do_downloadfile", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 6 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_downloadfile: wrong number of arguments ( " + args.size() + " for " + 6 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String filepath = args.get(2).toStr();
					String callback = args.get(3).toStr();
					String callback_params = args.get(4).toStr();
					boolean ssl_verify_peer = args.get(5).toInt() != 0 ? true : false;
					AsyncHttp pHttp = new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcDownload, url, args.get(1), null, filepath, callback, callback_params );
					return pHttp.getRetValue();
				} catch(Exception e) {
					LOG.ERROR("do_downloadfile failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});		

		klass.getSingletonClass().defineMethod("do_uploadfile", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 7 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_uploadfile: wrong number of arguments ( " + args.size() + " for " + 7 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String body = args.get(2).toStr();
					String filepath = args.get(3).toStr();
					String callback = args.get(4).toStr();
					String callback_params = args.get(5).toStr();
					boolean ssl_verify_peer = args.get(6).toInt() != 0 ? true : false;
					AsyncHttp pHttp = new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcUpload, url, args.get(1), body, filepath, callback, callback_params );
					return pHttp.getRetValue();
				} catch(Exception e) {
					LOG.ERROR("do_uploadfile failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});		

		klass.getSingletonClass().defineMethod("cancel", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					String cancel_callback = arg.toStr();
					AsyncHttp.cancelRequest(cancel_callback, false);
				} catch(Exception e) {
					LOG.ERROR("cancel failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		

	}
	
}

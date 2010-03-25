package com.rho.net;

import com.rho.*;
import com.rho.rjson.RJSONTokener;

import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.*;

import java.io.IOException;

public class AsyncHttp extends RhoThread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("AsyncHttp");

	private static Object m_mxInstances = new Object();
	private static Vector/*<CAsyncHttp*>*/ m_arInstances = new Vector();
	private RhoClassFactory m_ptrFactory;

	private NetRequest m_pNetRequest;
	private NetResponse m_pNetResponse;
	private Hashtable/*<String,String>*/ m_mapHeaders = new Hashtable();

	private String m_strUrl, m_strBody, m_strCallback, m_strCallbackParams;
    boolean m_bFinished = false;
    
	private RubyValue m_valBody;
	public final static int  hcGet = 0, hcPost=1, hcDownload=2, hcUpload =3;
	private int m_eCmd;
	public  static boolean m_bNoThreaded;
	
	private RhodesApp RHODESAPP(){ return RhodesApp.getInstance(); }
	
	AsyncHttp(RhoClassFactory factory, int eCmd,
		    String url, RubyValue headers, String body, String callback, String callback_params)
	{
		super(factory);
		
	    m_ptrFactory = factory;
	    m_strUrl = url != null ? url : "";
	    m_strBody = body != null ? body : "";
	    m_strCallback = callback != null ? callback : "";
	    m_strCallbackParams = callback_params != null ? callback_params : "";
	    m_eCmd = eCmd;

	    m_mapHeaders = RhoRuby.enum_strhash(headers);

	    addNewObject(this);	    

	    if (m_bNoThreaded)
	        run();
	    else
	        start(epLow);
	}

	void cancel()
	{
	    if (m_pNetRequest!=null && !m_pNetRequest.isCancelled())
	        m_pNetRequest.cancel();

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
	
	static void cancelRequest(String szCallback)
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
	                ((AsyncHttp)m_arInstances.elementAt(i)).cancel();
	        }else
	        {
	            for (int i = 0; i < (int)m_arInstances.size(); i++ )
	            {
	                if ( ((AsyncHttp)m_arInstances.elementAt(i)).m_strCallback.compareTo(szCallback) == 0 )
	                	((AsyncHttp)m_arInstances.elementAt(i)).cancel();    
	            }
	        }
	    }
	}

	public void run()
	{
		LOG.INFO("RhoHttp thread start.");

	    m_pNetRequest = m_ptrFactory.createNetRequest();

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
		        m_pNetResponse = m_pNetRequest.pullFile(m_strUrl, m_strBody, null, m_mapHeaders);
		        break;
	
		    case hcUpload:
		        m_pNetResponse = m_pNetRequest.pushFile(m_strUrl, m_strBody, null, m_mapHeaders);
		        break;
		    }
		    
		    if ( !m_pNetRequest.isCancelled() && m_strCallback.length() > 0)
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

	void callNotify(NetResponse resp, int nError )
	{
	    String strBody = "rho_callback=1";
	    strBody += "&status=";
	    if ( nError > 0 )
	    {
	    	strBody += "error&error_code=" + nError;
	    }else
	    {
	    	if ( resp.isOK() )
	    		strBody += "ok";
		    else
		    {
		    	strBody += "error&error_code=";
		        /*if ( !resp.isResponseRecieved())
		            strBody += RhoRuby.ERR_NETWORK;
		        else */if ( resp.isUnathorized() )
		            strBody += RhoRuby.ERR_UNATHORIZED;
		        else
			        strBody += RhoRuby.ERR_REMOTESERVER;
	
		        //if ( resp.isResponseRecieved())
			        strBody += "&http_error=" + resp.getRespCode();
		    }

	        String cookies = resp.getCookies();
	        if (cookies.length()>0)
	            strBody += "&cookies=" + URI.urlEncode(cookies);
	    	
	    	strBody += "&" + makeHeadersString();
	    	strBody += "&" + RHODESAPP().addCallbackObject(m_valBody, "body");
	    }
	    
	    if ( m_strCallbackParams.length() > 0 )
	        strBody += "&" + m_strCallbackParams;
	    	
/*
	    if ( m_bNoThreaded )
	    {
	        const char* szName = strrchr(m_strCallback.c_str(), '/');
	        if (!szName)
	            szName = m_strCallback.c_str();
	        else
	            szName++;

	        String strName = "C_";
	        strName += szName;
	        rho_ruby_set_const( strName.c_str(), strBody.c_str());
	    }else
	    {*/
	    try{
	        NetRequest pNetRequest = m_ptrFactory.createNetRequest();

	        String strFullUrl = pNetRequest.resolveUrl(m_strCallback);
	        NetResponse resp1 = pNetRequest.pushData( strFullUrl, strBody, null );
	        if ( !resp1.isOK() )
	            LOG.ERROR("AsyncHttp notification failed. Code: " + resp1.getRespCode() + "; Error body: " + resp1.getCharData() );
        }catch(Exception exc)
        {
        	LOG.ERROR("Async http callback failed.", exc);
        }

	    RHODESAPP().delCallbackObject(m_valBody);
	}
	
	public static void initMethods(RubyModule klass) {
		klass.getSingletonClass().defineMethod("do_get", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 4 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_get: wrong number of arguments ( " + args.size() + " for " + 4 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String callback = args.get(2).toStr();
					String callback_params = args.get(3).toStr();
					new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcGet, url, args.get(1), null, callback, callback_params );
				} catch(Exception e) {
					LOG.ERROR("do_get failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("do_post", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 5 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_post: wrong number of arguments ( " + args.size() + " for " + 5 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String body = args.get(2).toStr();
					String callback = args.get(3).toStr();
					String callback_params = args.get(4).toStr();
					new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcPost, url, args.get(1), body, callback, callback_params );
				} catch(Exception e) {
					LOG.ERROR("do_post failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		

		klass.getSingletonClass().defineMethod("do_downloadfile", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 5 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_downloadfile: wrong number of arguments ( " + args.size() + " for " + 5 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String filepath = args.get(2).toStr();
					String callback = args.get(3).toStr();
					String callback_params = args.get(4).toStr();
					new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcDownload, url, args.get(1), filepath, callback, callback_params );
				} catch(Exception e) {
					LOG.ERROR("do_downloadfile failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		

		klass.getSingletonClass().defineMethod("do_uploadfile", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block )
			{
				if ( args.size() != 5 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in AsyncHttp.do_uploadfile: wrong number of arguments ( " + args.size() + " for " + 5 + " )");			
				
				try {
					String url = args.get(0).toStr();
					String filepath = args.get(2).toStr();
					String callback = args.get(3).toStr();
					String callback_params = args.get(4).toStr();
					new AsyncHttp(new RhoClassFactory(), AsyncHttp.hcUpload, url, args.get(1), filepath, callback, callback_params );
				} catch(Exception e) {
					LOG.ERROR("do_uploadfile failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		

		klass.getSingletonClass().defineMethod("cancel", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					String cancel_callback = arg.toStr();
					AsyncHttp.cancelRequest(cancel_callback);
				} catch(Exception e) {
					LOG.ERROR("cancel failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		

		klass.getSingletonClass().defineMethod("set_threaded_mode", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					int b = arg.toInt();
					AsyncHttp.m_bNoThreaded = (b==0);
				} catch(Exception e) {
					LOG.ERROR("set_threaded_mode failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
				return RubyConstant.QNIL;
			}
		});		
		
	}
	
}
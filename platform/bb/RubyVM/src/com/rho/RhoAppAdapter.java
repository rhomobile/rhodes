package com.rho;

import java.io.IOException;
import com.rho.net.NetResponse;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;

public class RhoAppAdapter 
{
	public static final int ERR_NONE = 0;
	public static final int ERR_NETWORK = 1;
	public static final int ERR_REMOTESERVER = 2;
	public static final int ERR_RUNTIME = 3;
	public static final int ERR_UNEXPECTEDSERVERRESPONSE = 4;
	public static final int ERR_DIFFDOMAINSINSYNCSRC = 5;
	public static final int ERR_NOSERVERRESPONSE = 6;
	public static final int ERR_CLIENTISNOTLOGGEDIN = 7;
	public static final int ERR_CUSTOMSYNCSERVER = 8;
	public static final int ERR_UNATHORIZED = 9;
	public static final int ERR_CANCELBYUSER = 10;
	public static final int ERR_SYNCVERSION = 11;
	public static final int ERR_GEOLOCATION = 12;
	
	public static final RubyID loadServerSources_mid = RubyID.intern("load_server_sources");
	
	static RubyClass m_classRhoError;
	static RubyMethod m_RhoError_err_message;
	static RubyClass m_classRhoMessages;
	static RubyMethod m_RhoMessages_get_message;
		
	public static int getNetErrorCode(Exception exc)
	{
		if ( exc instanceof IOException )
		{
			String strMsg = exc.getMessage(); 
			
            return strMsg != null && (strMsg.indexOf("timed out") >= 0 || strMsg.indexOf("Timed out") >= 0)
	    		? ERR_NOSERVERRESPONSE : ERR_NETWORK;	    	
		}
		
		return ERR_NONE;
	}

	public static int getErrorCode(Exception exc)
	{
		return ERR_RUNTIME; 
	}
	
	public static int  getErrorFromResponse(NetResponse resp)
	{
	    if ( resp.isUnathorized() )
	    	return ERR_UNATHORIZED;

	    if ( !resp.isOK() )
	    	return ERR_REMOTESERVER;

	    return ERR_NONE;
	}
	
	public static String getErrorText(int nError)
	{
		if ( nError == ERR_NONE )
			return "";
		
		if ( m_classRhoError == null )
		{
			RubyModule modRho = (RubyModule)RubyRuntime.ObjectClass.getConstant("Rho");
			m_classRhoError = (RubyClass)modRho.getConstant("RhoError");
			m_RhoError_err_message = m_classRhoError.findMethod( RubyID.intern("err_message") );
		}
		
		RubyValue res = m_RhoError_err_message.invoke( m_classRhoError, ObjectFactory.createInteger(nError), null );
		
		return res.toStr();
	}

	public static String getMessageText(String strName)
	{
		if ( m_classRhoMessages == null )
		{
			RubyModule modRho = (RubyModule)RubyRuntime.ObjectClass.getConstant("Rho");
			m_classRhoMessages = (RubyClass)modRho.getConstant("RhoMessages");
			m_RhoMessages_get_message = m_classRhoMessages.findMethod( RubyID.intern("get_message") );
		}
		
		RubyValue res = m_RhoMessages_get_message.invoke( m_classRhoMessages, ObjectFactory.createString(strName), null );
		
		return res.toStr();
	}

	public static void loadServerSources(String strData)
	{
		RubyAPI.callPublicOneArgMethod(RhoRuby.getFrameworkObj(), ObjectFactory.createString(strData), null, loadServerSources_mid);
	}
	
}

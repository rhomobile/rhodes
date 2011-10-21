/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
	public static final RubyID loadAllSyncSources_mid = RubyID.intern("load_all_sync_sources");
	public static final RubyID resetDBOnSyncUserChanged_mid = RubyID.intern("reset_db_on_sync_user_changed");

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

	public static void loadAllSyncSources()
	{
		RubyAPI.callPublicNoArgMethod(RhoRuby.getFrameworkObj(), null, loadAllSyncSources_mid);
	}

	public static void resetDBOnSyncUserChanged()
	{
		RubyAPI.callPublicNoArgMethod(RhoRuby.getFrameworkObj(), null, resetDBOnSyncUserChanged_mid);
	}

}

package com.rho;

import com.rho.net.NetResponse;

public class RhoAppAdapter {

	public static final int ERR_CLIENTISNOTLOGGEDIN = 0;
	public static final int ERR_CUSTOMSYNCSERVER = 0;
	public static final int ERR_NONE = 0;
	public static final int ERR_REMOTESERVER = 0;
	public static final int ERR_RUNTIME = 0;
	public static final int ERR_UNEXPECTEDSERVERRESPONSE = 0;

	public static int getErrorFromResponse(NetResponse resp) {
		// TODO Auto-generated method stub
		return 0;
	}

	public static int getNetErrorCode(Exception exc) {
		// TODO Auto-generated method stub
		return 0;
	}

	public static String getMessageText(String string) {
		// TODO Auto-generated method stub
		return null;
	}

	public static void loadServerSources(String strSources) {
		// TODO Auto-generated method stub
		
	}

	public static void resetDBOnSyncUserChanged() {
		// TODO Auto-generated method stub
		
	}

}

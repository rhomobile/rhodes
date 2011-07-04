package com.rhomobile.rhosync;

import java.io.Closeable;
import java.io.IOException;

public class RhoSyncClient implements Closeable{
	public native void setSyncServer(String url);

	public native void setThreadedMode(boolean mode);
	//public native boolean getThreadedMode();

	public native void setPollInterval(int interval);
	public native int getPollInterval();

	public native void setBulkSyncState(int state);
	public native int getBulkSyncState();

	public native void setConfigString(String name, String param);
	public native String getConfigString(String name);

	public native void initDatabase();
	public native void addModels(RhomModel[] models);
	public native void databaseFullResetAndLogout();
	public native boolean isLoggedIn();
	
	public native RhoSyncNotify loginWithUser(String user, String pass);
	public native RhoSyncNotify syncAll();

	/*TODO:
	RhoSyncNotify search(...);
	*/
	
	public static native void nativeInit(String root, String sqliteJournals);
	
	@Override
	public void close() throws IOException {
	}
	
	static {
		System.loadLibrary("rhosyncclient");
	}

}

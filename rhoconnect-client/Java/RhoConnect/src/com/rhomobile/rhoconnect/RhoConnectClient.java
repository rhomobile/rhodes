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


package com.rhomobile.rhoconnect;

public class RhoConnectClient{
	private static RhoConnectClient sInstance;
	private synchronized void createInstance() {
		if (sInstance != null)
			throw new java.lang.IllegalStateException("RhoConnectClient instance already initialized.");
		sInstance = this;
	}

	public native void initialize(RhomModel models[]);
	private native void destroy();

	public native void setSyncServer(String url);

	public native void setThreadedMode(boolean mode);
	//public native boolean getThreadedMode();

	public native void setPollInterval(int interval);
	public native int getPollInterval();

	public native void setBulkSyncState(int state);
	public native int getBulkSyncState();

	public native void setConfigString(String name, String param);
	public native String getConfigString(String name);

	public native void databaseFullResetAndLogout();
	public native boolean isLoggedIn();

	public native void setObjectNotification(RhoConnectObjectNotify.IDelegate callback);
	public native void clearObjectNotification();
	public native void addObjectNotify(int stcId, String object);

	public native void setNotification(RhoConnectNotify.IDelegate callback);
	public native void clearNotification();

	public native RhoConnectNotify loginWithUserSync(String user, String pass);
	public native void loginWithUserAsync(String user, String pass, RhoConnectNotify.IDelegate callback);
	public native RhoConnectNotify syncAll();


	/*TODO:
	RhoConnectNotify search(...);
	*/

	public static native void nativeInit();

	public RhoConnectClient() { createInstance(); }

	public synchronized void close(){
		destroy();
		sInstance = null;
	}

//	static {
//	}

}

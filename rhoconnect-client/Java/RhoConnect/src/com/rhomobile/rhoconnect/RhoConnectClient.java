/*
 ============================================================================
 Author	    : Alexey Tikhvinsky
 Copyright  : Copyright (C) 2011 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */

package com.rhomobile.rhoconnect;

import java.io.Closeable;
import java.io.IOException;

public class RhoConnectClient implements Closeable {
	public native void initialize(RhomModel models[]);
	public native void destroy();
	
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
	public native void databaseFullResetAndLogout();
	public native boolean isLoggedIn();
	
	public native RhoConnectNotify loginWithUser(String user, String pass);
	public native RhoConnectNotify syncAll();

	/*TODO:
	RhoConnectNotify search(...);
	*/
	
	public static native void nativeInit(/*String root, String sqliteJournals*/);
	
	@Override
	public void close() throws IOException {
		destroy();
	}
	
//	static {
//	}

}

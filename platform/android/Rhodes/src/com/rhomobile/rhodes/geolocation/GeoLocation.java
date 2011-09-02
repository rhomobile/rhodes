/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

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
package com.rhomobile.rhodes.geolocation;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.util.PerformOnUiThread;

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static volatile GeoLocationImpl locImpl = null;
	private static volatile int inactivityTimerId = 0;
	
	static long getInactivityTimeout() {
		int sec = RhoConf.getInt("geo_location_inactivity_timeout");
		if (sec == 0)
			return 600000; //10 min
		else
			return sec*1000;
	}
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void checkState() throws IllegalAccessException {
		if (!Capabilities.GPS_ENABLED)
			throw new IllegalAccessException("Capability GPS disabled");
	}
	
	private static void updateInactivityTimer() {
		final long inactivityTimeout = getInactivityTimeout();
		Logger.T(TAG, "Updating inactivity timer: " + inactivityTimeout + "ms");
		PerformOnUiThread.exec(new Runnable() {
			int lastId = ++inactivityTimerId;
			public void run() {
				if ((inactivityTimerId != lastId) || (locImpl == null))
					return;

				Logger.T(TAG, "Stop geolocation by inactivity timeout: " + inactivityTimeout + "ms");
				stop();
			}
		}, inactivityTimeout);
	}

	private static GeoLocationImpl getImpl() {
		if (locImpl == null) {
			synchronized (GeoLocation.class) {
				if (locImpl == null) {
					Logger.T(TAG, "Creating GeoLocationImpl instance >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
					locImpl = new GeoLocationImpl(getInactivityTimeout());
					Logger.T(TAG, "GeoLocationImpl instance has created <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
					locImpl.start();
					Logger.T(TAG, "GeoLocation has started ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
				}
			}
		}
		updateInactivityTimer();
		return locImpl;
	}
	
	public static void stop() {
		Logger.T(TAG, "stop");
		try {
			if (locImpl == null)
				return;
			synchronized(GeoLocation.class) {
				if (locImpl == null)
					return;
				locImpl.stop();
				locImpl = null;
			}
		}
		catch (Exception e) {
			reportFail("stop", e);
		}
	}
	
	public static boolean isAvailable() {
		Logger.T(TAG, "isAvailable...");
		try {
			boolean result = false;
			if (locImpl != null) {
				checkState();
				result = getImpl().isAvailable();
			}
			Logger.T(TAG, "Geo location service is " + (result ? "" : "not ") + "available");
			return result;
		}
		catch (Exception e) {
			reportFail("isAvailable", e);
		}
		return false;
	}
	
	public static double getLatitude() {
		try {
			checkState();
			Logger.T(TAG, "getLatitude");
			return getImpl().getLatitude();
		}
		catch (Exception e) {
			reportFail("getLatitude", e);
		}
		return 0.0;
	}

	public static double getLongitude() {
		try {
			checkState();
			Logger.T(TAG, "getLongitude");
			return getImpl().getLongitude();
		}
		catch (Exception e) {
			reportFail("getLongitude", e);
		}
		return 0.0;
	}
	
	public static float getAccuracy() {
		try {
			checkState();
			Logger.T(TAG, "getAccuracy");
			return getImpl().getAccuracy();
		}
		catch (Exception e) {
			reportFail("getAccuracy", e);
		}
		return 0;
	}

	public static boolean isKnownPosition() {
		try {
			checkState();
			Logger.T(TAG, "isKnownPosition");
			return getImpl().isKnownPosition();
		}
		catch (Exception e) {
			reportFail("isKnownPosition", e);
		}
		return false;
	}
	
	public static void setTimeout(int nsec) {
		try {
			if (nsec < 0) {
				Logger.E(TAG, "setTimeout: wrong parameter: " + nsec);
				return;
			}
			
			checkState();
			Logger.T(TAG, "setTimeout: " + nsec + "s");
			getImpl().setTimeout(nsec);
		}
		catch (Exception e) {
			reportFail("setTimeout", e);
		}
	}

}

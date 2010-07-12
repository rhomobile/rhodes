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

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static GeoLocationImpl locImpl = null;
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	private static void checkState() throws IllegalAccessException {
		if (!Capabilities.GPS_ENABLED)
			throw new IllegalAccessException("Capability GPS disabled");
	}
	
	private static void init() {
		if (locImpl != null)
			return;
		
		synchronized (TAG) {
			if (locImpl == null)
				locImpl = new GeoLocationImpl();
		}
	}
	
	public static void stop() {
		try {
			Logger.T(TAG, "stop");
			if (locImpl == null)
				return;
			synchronized (TAG) {
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
		try {
			Logger.T(TAG, "isAvailable...");
			boolean result = false;
			if (locImpl != null) {
				checkState();
				init();
				result = locImpl.isAvailable();
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
			init();
			return locImpl.GetLatitude();
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
			init();
			return locImpl.GetLongitude();
		}
		catch (Exception e) {
			reportFail("getLongitude", e);
		}
		
		return 0.0;
	}

	public static boolean isKnownPosition() {
		try {
			checkState();
			Logger.T(TAG, "isKnownPosition");
			init();
			return locImpl.isKnownPosition();
		}
		catch (Exception e) {
			reportFail("isKnownPosition", e);
		}
		
		return false;
	}

}

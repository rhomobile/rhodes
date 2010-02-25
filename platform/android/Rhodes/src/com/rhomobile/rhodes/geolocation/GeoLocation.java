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

import com.rhomobile.rhodes.Logger;

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static double getLatitude() {
		try {
			Logger.T(TAG, "GeoLocation.GetLatitude");
			
			if (locImpl != null)
				  return locImpl.GetLatitude();
		}
		catch (Exception e) {
			reportFail("getLatitude", e);
		}
			
		return 0.0;
	}

	public static double getLongitude() {
		try {
			Logger.T(TAG, "GeoLocation.GetLongitude");
			
			if (locImpl != null)
			  return locImpl.GetLongitude();
		}
		catch (Exception e) {
			reportFail("getLongitude", e);
		}
		
		return 0.0;
	}

	public static boolean isKnownPosition() {
		try {
			Logger.T(TAG, "GeoLocation.isKnownPosition");
			
			return locImpl != null && locImpl.isKnownPosition();
		}
		catch (Exception e) {
			reportFail("isKnownPosition", e);
		}
		
		return false;
	}

}

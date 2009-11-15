package com.rhomobile.rhodes.geolocation;

import com.rhomobile.rhodes.Logger;

public class GeoLocation {

	private static final String TAG = "GeoLocation";
	private static GeoLocationImpl locImpl = new GeoLocationImpl();
	
	public static double getLatitude() {
		Logger.T(TAG, "GeoLocation.GetLatitude");
		
		if (locImpl != null)
			  return locImpl.GetLatitude();
			
		return 0.0;
	}

	public static double getLongitude() {
		Logger.T(TAG, "GeoLocation.GetLongitude");
		
		if (locImpl != null)
		  return locImpl.GetLongitude();
		
		return 0.0;
	}

	public static boolean isKnownPosition() {
		Logger.T(TAG, "GeoLocation.isKnownPosition");
		
		return locImpl != null && locImpl.isKnownPosition();
	}

}
